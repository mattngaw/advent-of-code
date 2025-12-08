CFLAGS = -O3 -fno-vectorize -g
CFLAGS_DEBUG = $(CFLAGS) -g -DDEBUG

part1a:
	mkdir -p out
	clang $(CFLAGS) -o out/part1a src/part1a.c

part1a-debug:
	mkdir -p out
	clang $(CFLAGS_DEBUG) -o out/part1a-debug src/part1a.c

part1b:
	mkdir -p out
	clang $(CFLAGS) -o out/part1b src/part1b.c

part1b-debug:
	mkdir -p out
	clang $(CFLAGS_DEBUG) -o out/part1b-debug src/part1b.c

.PHONY: clean
clean:
	rm -f out/*
