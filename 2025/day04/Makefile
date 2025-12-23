CC          = clang
OUT_DIR     = out
SRC_DIR     = src

BASE_CFLAGS  = -O3 -fno-vectorize
CFLAGS       = $(BASE_CFLAGS) -g
CFLAGS_DEBUG = $(BASE_CFLAGS) -g -DDEBUG

# List of programs (without extension or path)
PROGRAMS = part1a part1b part1c part1d

# --------------------------------------------------------------------
# Target-specific extra flags
# Add per-target flags like this:
#   CFLAGS_<target> = <extra flags>
#
# Example: add -pthread only to part1c
CFLAGS_part1c = -pthread
# You can define more, e.g.:
# CFLAGS_part1a = -Wall
# CFLAGS_part1b = -Wextra
# --------------------------------------------------------------------

# Default target: build all release binaries
all: $(PROGRAMS:%=$(OUT_DIR)/%)

# Individual targets for convenience
part1a: $(OUT_DIR)/part1a
part1b: $(OUT_DIR)/part1b
part1c: $(OUT_DIR)/part1c
part1d: $(OUT_DIR)/part1d
part1a-debug: $(OUT_DIR)/part1a-debug
part1b-debug: $(OUT_DIR)/part1b-debug
part1c-debug: $(OUT_DIR)/part1c-debug
part1d-debug: $(OUT_DIR)/part1d-debug

# Pattern rule for normal builds
# $* is the stem (e.g. "part1c"), so CFLAGS_$* expands to CFLAGS_part1c
$(OUT_DIR)/%: $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_$*) -o $@ $<

# Pattern rule for debug builds (also supports per-target flags)
$(OUT_DIR)/%-debug: $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) $(CFLAGS_DEBUG) $(CFLAGS_$*) -o $@ $<

# Ensure output directory exists (order-only prerequisite)
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

.PHONY: clean
clean:
	rm -f $(OUT_DIR)/*
