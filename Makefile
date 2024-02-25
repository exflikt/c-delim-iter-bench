# I wrote this makefile for learning purposes, so you might find it needlessly
# complex and over-engineered to understand. Please do not take it seriously.

SHELL = /bin/bash

CC = gcc
CFLAGS = -Wall -Wextra
CFLAGS += -O3

BIN_DIR = bin/
OUTPUT_DIR = output/
TEST = diff -quB --from-file
BENCH = hyperfine --warmup=1 -N

SUFFIXES = iter iter-struct strtok
# split one long string, split many short strings
VARIANTS = sols smss

help:
	@echo "Recipes:"
	@echo "  compile  Compile binaries"
	@echo "  run      Compile and run all binaries"
	@echo "  test     Test sols and smss"
	@echo "  bench    Benchmark sols and smss (\`hyperfine\` must be installed)"
	@echo "  clean    Remove binaries and output texts"
	@echo "  help     Show this message"

compile: $(foreach variant,$(VARIANTS),$(foreach suffix,$(SUFFIXES),$(BIN_DIR)$(variant)-$(suffix)))

run: $(foreach variant,$(VARIANTS),$(foreach suffix,$(SUFFIXES),run-$(variant)-$(suffix)))

test: $(foreach variant,$(VARIANTS),test-$(variant))

bench: $(foreach variant,$(VARIANTS),bench-$(variant))

clean:
	rm -rf $(BIN_DIR) $(OUTPUT_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)sols-%: split-one-long-string/%.c $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $<

$(BIN_DIR)smss-%: split-many-short-strings/%.c $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $<

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

run-%: $(BIN_DIR)% $(OUTPUT_DIR)
	./$< > $(OUTPUT_DIR)$*
	@echo "stdout is saved to $(OUTPUT_DIR)$*"

test-%: $(foreach suffix,$(SUFFIXES),$(BIN_DIR)%-$(suffix))
	$(TEST) $(foreach binary,$^,<(./$(binary)))

bench-%: $(foreach suffix,$(SUFFIXES),$(BIN_DIR)%-$(suffix))
	$(BENCH) $^

