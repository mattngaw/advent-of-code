part1a:
	mkdir -p out
	clang -O3 -g -o out/part1a src/part1a.c

part1a-debug:
	mkdir -p out
	clang -O3 -g -DDEBUG -o out/part1a-debug src/part1a.c

.PHONY: clean
clean:
	rm -f out/*
