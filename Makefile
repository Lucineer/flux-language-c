CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2

all: test

test: language.c test_language.c language.h
	$(CC) $(CFLAGS) -o test_language test_language.c language.c -lm
	./test_language

clean:
	rm -f test_language
