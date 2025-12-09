/*
 * @file part1c.c
 * @brief A multithreaded improvement over part1b.
 *
 * Breaks down both the string preprocessing and convolution into N chunks of rows for N threads to
 * work on.
 *
 * Next optimization to try is tiling via squares rather than bundles of rows.
 */

#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE
#endif

#define LIMIT (4)

#define KERNEL_ROWS (3)
#define KERNEL_COLS (3)

typedef unsigned char T;

NOINLINE char *read(const char *filename, size_t *len) {
    FILE *fptr;
    char *map;
    size_t fileSize;

    fptr = fopen(filename, "rt");
    fseek(fptr, 0, SEEK_END);
    fileSize = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    map = (char *)malloc(fileSize + 1);
    *len = fread(map, sizeof(char), fileSize, fptr);
    map[fileSize] = '\0';

    fclose(fptr);

    return map;
}

typedef struct {
    char *map;
    T *input;
    int startRow;
    int rows;
    int cols;
    int paddedCols;
} PreprocessArgs;

void *preprocessThread(void *args) {
    PreprocessArgs *preprocessArgs = (PreprocessArgs *)args;

    char *map = preprocessArgs->map;
    T *input = preprocessArgs->input;
    int startRow = preprocessArgs->startRow;
    int rows = preprocessArgs->rows;
    int cols = preprocessArgs->cols;
    int paddedCols = preprocessArgs->paddedCols;

    for (size_t i = startRow, pi = startRow + 1; i < startRow + rows; i++, pi++) {
        for (size_t j = 0, pj = 1; j < cols; j++, pj++) {
            input[pi * paddedCols + pj] = map[i * (cols + 1) + j] == '@';
        }
    }

    return NULL;
}

NOINLINE T *preprocess(int numThreads, char *map, size_t len, int *inputRows, int *inputCols) {
    int rows, cols;
    int paddedRows, paddedCols;
    int r, c;
    T *input;
    int rowsPerThread, firstAdditionalRows;

    rows = 0;
    cols = 0;

    for (size_t i = 0; map[i] != '\n'; i++) {
        cols++;
    }

    rows = len / (cols + 1);

    r = 1;
    c = 1;
    paddedRows = rows + 2;
    paddedCols = cols + 2;

    input = (T *)malloc(paddedRows * paddedCols * sizeof(T));

    rowsPerThread = rows / numThreads;
    firstAdditionalRows = rows % numThreads;

    for (size_t i = 0; i < paddedCols; i++) {
        input[i] = 0;
        input[(paddedRows - 1) * paddedCols + i] = 0;
    }
    for (size_t i = 0; i < paddedRows; i++) {
        input[paddedCols * i] = 0;
        input[(paddedCols - 1) * i + (paddedCols + 1)] = 0;
    }

    pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
    PreprocessArgs *args = malloc(numThreads * sizeof(PreprocessArgs));

    for (int i = 0; i < numThreads; i++) {
        args[i].map = map;
        args[i].input = input;
        args[i].rows = rowsPerThread;
        args[i].cols = cols;
        args[i].paddedCols = paddedCols;
    }

    args[0].startRow = 0;
    args[0].rows = rowsPerThread + firstAdditionalRows;
    for (int i = 1; i < numThreads; i++) {
        args[i].startRow = i * rowsPerThread + firstAdditionalRows;
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_create(&threads[i], NULL, preprocessThread, (void *)&args[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    *inputRows = paddedRows;
    *inputCols = paddedCols;
    return input;
}

typedef struct {
    T *input;
    int startRow;
    int rows;
    int inputCols;
    int *localResult;
} ConvolveArgs;

void *convolveThread(void *args) {
    ConvolveArgs *convolveArgs = (ConvolveArgs *)args;

    T *input = convolveArgs->input;
    int startRow = convolveArgs->startRow;
    int rows = convolveArgs->rows;
    int inputCols = convolveArgs->inputCols;
    int *localResult = convolveArgs->localResult;

    int count;
    int result;

    result = 0;

    for (int ir = startRow; ir < startRow + rows; ir++) {
        for (int ic = 1; ic < inputCols - 1; ic++) {
            count =
                input[(ir - 1) * inputCols + (ic - 1)] + input[(ir - 1) * inputCols + (ic + 1)] +
                input[(ir - 1) * inputCols + (ic + 0)] + input[(ir + 0) * inputCols + (ic - 1)] +
                input[(ir + 0) * inputCols + (ic + 1)] + input[(ir + 1) * inputCols + (ic - 1)] +
                input[(ir + 1) * inputCols + (ic + 1)] + input[(ir + 1) * inputCols + (ic + 0)];
            result += input[ir * inputCols + ic] && count < LIMIT;
        }
    }

    *localResult = result;

    return NULL;
}

NOINLINE int convolve(int numThreads, T *input, int inputRows, int inputCols) {
    int count;
    int result;
    int rowsPerThread, firstAdditionalRows;

    rowsPerThread = (inputRows - 2) / numThreads;
    firstAdditionalRows = (inputRows - 2) % numThreads;

    pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
    ConvolveArgs *args = malloc(numThreads * sizeof(ConvolveArgs));
    int *localResults = malloc(numThreads * sizeof(int));

    for (int i = 0; i < numThreads; i++) {
        args[i].input = input;
        args[i].rows = rowsPerThread;
        args[i].inputCols = inputCols;
        args[i].localResult = &localResults[i];
    }

    args[0].startRow = 1;
    args[0].rows = rowsPerThread + firstAdditionalRows;
    for (int i = 1; i < numThreads; i++) {
        args[i].startRow = i * rowsPerThread + firstAdditionalRows + 1;
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_create(&threads[i], NULL, convolveThread, (void *)&args[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    result = 0;

    for (int i = 0; i < numThreads; i++) {
        result += localResults[i];
    }

    return result;
}

int main(int argc, char *argv[]) {
    char *map;
    size_t len;

    T *input;
    int inputRows, inputCols;

    int result;

    if (argc != 3) {
        printf("Usage: ./part1c <input-path> <threads>\n");
        return 1;
    }

    const char *filename = argv[1];
    int numThreads = atoi(argv[2]);

    map = read(filename, &len);
    input = preprocess(numThreads, map, len, &inputRows, &inputCols);
    result = convolve(numThreads, input, inputRows, inputCols);

    printf("%d\n", result);

    free(map);
    free(input);

    return 0;
}
