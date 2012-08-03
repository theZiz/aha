all: aha

aha: aha.c
	gcc -std=c99 $(CFLAGS) $(LDFLAGS) $(CPPFLAGS) aha.c -o aha

clean:
	rm aha
