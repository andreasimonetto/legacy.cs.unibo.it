include ../config.mk

INCLUDE = -I../include $(LIBUMPS_INCLUDE)
GCC = gcc -O3 -Wall -pedantic -ansi

TEST_UNEXP = $(wildcard *_test.unexp.c)
TEST_SRC = $(patsubst %.unexp.c,%.c,$(TEST_UNEXP))
TEST_OBJ = $(patsubst %.c,%,$(TEST_SRC))
all: $(TEST_OBJ)

clean: 
	-rm -f $(TEST_OBJ)

distclean: clean
	./build_unexp.sh -d

$(TEST_SRC): $(TEST_UNEXP)
	./build_unexp.sh

$(TEST_OBJ): %: %.c
	$(GCC) $(INCLUDE) -o $@ $<

.PHONY: all clean
