/*
 * @file part1b.c
 * @brief A zero-padded improvement over part1a that removes the conditional from the inner loop
 *
 * The previous implementation requires a complex conditional check in the innermost (and hottest)
 * loop to ensure that the exterior of the image is interpreted as empty space (no rolls).
 * Surrounding the original image with a perimeter of zeroes allows the inner conditional to be
 * removed.
 *
 * Since the image has now been transformed from NxN to (N+2)x(N+2), the memory access patterns of
 * some other loops have also changed.
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
    int padded_rows, padded_cols;
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

    r = 1;
    c = 1;
    padded_rows = rows + 2;
    padded_cols = cols + 2;
    input = (T *)calloc(padded_rows * padded_cols, sizeof(T));
    for (size_t i = 0; i < len; i++) {
        if (map[i] == '\n') {
            c = 1;
            r++;
        } else {
            if (map[i] == '@')
                input[r * padded_cols + c] = 1;
            c++;
        }
    }

    *inputRows = padded_rows;
    *inputCols = padded_cols;
    return input;
}

NOINLINE T *convolve(T *input, int inputRows, int inputCols, T *kernel, int kernelRows,
                     int kernelCols) {
    int outputRows, outputCols;
    T *output;
    int centerRow, centerCol;

    outputRows = inputRows;
    outputCols = inputCols;

    output = (T *)calloc(outputRows * outputCols, sizeof(T));

    centerRow = kernelRows / 2;
    centerCol = kernelCols / 2;

    for (int i = 0; i < inputRows; i++) {
        for (int j = 0; j < inputCols; j++) {
            for (int kr = 0; kr < kernelRows; kr++) {
                for (int kc = 0; kc < kernelCols; kc++) {
                    int or = i - centerRow + kr;
                    int oc = j - centerCol + kc;

                    output[i * outputCols + j] +=
                        input[or * inputCols + oc] * kernel[kr * kernelCols + kc];
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

    T *kernel;

    int count;

    if (argc != 2) {
        printf("Usage: ./part1a <input-path>\n");
        return 1;
    }

    const char *filename = argv[1];

    kernel = (T *)malloc(KERNEL_ROWS * KERNEL_COLS * sizeof(T));
    for (int i = 0; i < KERNEL_ROWS * KERNEL_COLS; i++)
        kernel[i] = 1;
    kernel[4] = 0;

    map = read(filename, &len);
    input = preprocess(map, len, &inputRows, &inputCols);

    output = convolve(input, inputRows, inputCols, kernel, KERNEL_ROWS, KERNEL_COLS);
    outputRows = inputRows;
    outputCols = inputCols;

    count = tally(output, input, outputRows, outputCols);

    printf("%d\n", count);

    free(map);
    free(input);
    free(output);
    free(kernel);

    return 0;
}
