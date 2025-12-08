CC          = clang
OUTDIR      = out
SRC_DIR     = src

BASE_CFLAGS = -O3 -fno-vectorize
CFLAGS      = $(BASE_CFLAGS) -g
CFLAGS_DEBUG = $(BASE_CFLAGS) -g -DDEBUG

# List of programs (without extension or path)
PROGRAMS = part1a part1b

# Default target: build all release binaries
all: $(PROGRAMS:%=$(OUTDIR)/%)

# Individual targets for convenience
part1a: $(OUTDIR)/part1a
part1b: $(OUTDIR)/part1b
part1a-debug: $(OUTDIR)/part1a-debug
part1b-debug: $(OUTDIR)/part1b-debug

# Pattern rule for normal builds
$(OUTDIR)/%: $(SRC_DIR)/%.c | $(OUTDIR)
	$(CC) $(CFLAGS) -o $@ $<

# Pattern rule for debug builds
$(OUTDIR)/%-debug: $(SRC_DIR)/%.c | $(OUTDIR)
	$(CC) $(CFLAGS_DEBUG) -o $@ $<

# Ensure output directory exists (order-only prerequisite)
$(OUTDIR):
	mkdir -p $(OUTDIR)

.PHONY: clean
clean:
	rm -f $(OUTDIR)/*
