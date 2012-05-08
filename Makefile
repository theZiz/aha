all: aha

aha: aha.c
	gcc -std=c99 aha.c -o aha
