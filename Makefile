# This file just exists for some quality of life reasons because
# I edit with vim.

all:
	@make -sC build
.PHONY: all

clean:
	@make clean -sC build
.PHONY: clean

check:
	@make check lint -sC build
.PHONY: test

doc:
	@make doc -sC build
.PHONY: doc
