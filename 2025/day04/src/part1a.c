/*
 * @file part1a.c
 * @brief A single-threaded, convolution-based implementation of AoC 2025 Day 4 Part 1
 *
 * This implementation uses a convolution to transform the input image into an image describing the
 * number of rolls surrounding each roll. This intermediate image is then scanned for rolls with
 * less than four neighbors.
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define KERNEL_ROWS (3)
#define KERNEL_COLS (3)
#define LIMIT (4)

#ifdef DEBUG
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE
#endif

typedef int T;

NOINLINE char *read(const char *filename, size_t *len) {
    FILE *fptr;
    char *map;
    size_t file_size;
    size_t bytes_read;

    fptr = fopen(filename, "rt");
    fseek(fptr, 0, SEEK_END);
    file_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    map = (char *)malloc(file_size + 1);
    *len = fread(map, sizeof(char), file_size, fptr);
    map[file_size] = '\0';

    fclose(fptr);

    return map;
}

NOINLINE T *preprocess(char *map, size_t len, int *inputRows, int *inputCols) {
    int rows, cols;
    int r, c;
    T *input;

    rows = 0;
    cols = 0;

    for (size_t i = 0; map[i] != '\n'; i++) {
        cols++;
    }

    for (size_t i = 0; i < len; i++) {
        if (map[i] == '\n')
            rows++;
    }

    r = 0;
    c = 0;
    input = (T *)calloc(rows * cols, sizeof(T));
    for (size_t i = 0; i < len; i++) {
        if (map[i] == '\n') {
            c = 0;
            r++;
        } else {
            if (map[i] == '@') {
                input[r * cols + c] = 1;
            } else {
                input[r * cols + c] = 0;
            }
            c++;
        }
    }

    *inputRows = rows;
    *inputCols = cols;
    return input;
}

NOINLINE T *convolve(T *input, int inputRows, int inputCols) {
    int outputRows, outputCols;
    T *output;
    int centerRow, centerCol;

    T kernel[KERNEL_ROWS * KERNEL_COLS] = {1, 1, 1, 1, 0, 1, 1, 1, 1};

    outputRows = inputRows;
    outputCols = inputCols;

    output = (T *)calloc(outputRows * outputCols, sizeof(T));

    centerRow = KERNEL_ROWS / 2;
    centerCol = KERNEL_COLS / 2;

    for (int i = 0; i < inputRows; i++) {
        for (int j = 0; j < inputCols; j++) {
            for (int kr = 0; kr < KERNEL_ROWS; kr++) {
                for (int kc = 0; kc < KERNEL_COLS; kc++) {
                    int or = i - centerRow + kr;
                    int oc = j - centerCol + kc;

                    if (or >= 0 && or < outputRows && oc >= 0 && oc < outputCols)
                        output[i * outputCols + j] +=
                            input[or * inputCols + oc] * kernel[kr * KERNEL_COLS + kc];
                }
            }
        }
    }

    return output;
}

NOINLINE int tally(T *output, T *input, int outputRows, int outputCols) {
    int count;

    count = 0;
    for (int i = 0; i < outputRows * outputCols; i++) {
        if (output[i] < LIMIT && input[i])
            count++;
    }

    return count;
}

int main(int argc, char *argv[]) {
    char *map;
    size_t len;

    T *input;
    int inputRows, inputCols;

    T *output;
    int outputRows, outputCols;

    int count;

    if (argc != 2) {
        printf("Usage: ./part1a <input-path>\n");
        return 1;
    }

    const char *filename = argv[1];

    map = read(filename, &len);
    input = preprocess(map, len, &inputRows, &inputCols);

    output = convolve(input, inputRows, inputCols);
    outputRows = inputRows;
    outputCols = inputCols;
    count = tally(output, input, outputRows, outputCols);

    printf("%d\n", count);

    free(map);
    free(input);
    free(output);

    return 0;
}
