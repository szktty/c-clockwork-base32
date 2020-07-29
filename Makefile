all:
	cc -o test base32.c test.c

clean:
	rm -rf test
