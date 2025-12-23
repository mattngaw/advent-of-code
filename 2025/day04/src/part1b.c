/*
 * @file part1b.c
 * @brief A mostly-branchless improvement over part1a that removes many conditionals, resulting in a
 * 6x speedup (on my machine)
 *
 * The previous implementation requires a complex conditional check in the innermost (and hottest)
 * convolve loop to ensure that the exterior of the image is interpreted as empty space (no rolls).
 * Surrounding the original image with a perimeter of zeroes allows the inner conditional to be
 * removed.
 *
 * Since the image has now been transformed from NxN to (N+2)x(N+2), the memory access patterns of
 * some other loops have also changed.
 *
 * The preprocess function was also rewritten in a branchless way, relying heavily on
 * bitwise/logical operators and conditional moves.
 *
 * The counting of candidate rolls (previously tally) was rolled into the main loop of the convolve
 * function.
 */

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

NOINLINE T *preprocess(char *map, size_t len, int *inputRows, int *inputCols) {
    int rows, cols;
    int paddedRows, paddedCols;
    int r, c;
    T *input;

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
    input = (T *)calloc(paddedRows * paddedCols, sizeof(T));
    for (size_t i = 0, pi = 1; i < rows; i++, pi++) {
        for (size_t j = 0, pj = 1; j < cols; j++, pj++) {
            input[pi * paddedCols + pj] = map[i * (cols + 1) + j] == '@';
        }
    }

    *inputRows = paddedRows;
    *inputCols = paddedCols;
    return input;
}

NOINLINE int convolve(T *input, int inputRows, int inputCols) {
    int count;
    int result;

    result = 0;

    for (int ir = 1; ir < inputRows - 1; ir++) {
        for (int ic = 1; ic < inputCols - 1; ic++) {
            count =
                input[(ir - 1) * inputCols + (ic - 1)] + input[(ir - 1) * inputCols + (ic + 1)] +
                input[(ir - 1) * inputCols + (ic + 0)] + input[(ir + 0) * inputCols + (ic - 1)] +
                !input[(ir + 0) * inputCols + (ic + 0)] * LIMIT +
                input[(ir + 0) * inputCols + (ic + 1)] + input[(ir + 1) * inputCols + (ic - 1)] +
                input[(ir + 1) * inputCols + (ic + 1)] + input[(ir + 1) * inputCols + (ic + 0)];
            result += count < LIMIT;
        }
    }

    return result;
}

int main(int argc, char *argv[]) {
    char *map;
    size_t len;

    T *input;
    int inputRows, inputCols;

    int result;

    if (argc != 2) {
        printf("Usage: ./part1b <input-path>\n");
        return 1;
    }

    const char *filename = argv[1];

    map = read(filename, &len);
    input = preprocess(map, len, &inputRows, &inputCols);
    result = convolve(input, inputRows, inputCols);

    printf("%d\n", result);

    free(map);
    free(input);

    return 0;
}
