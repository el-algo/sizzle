all: main.bin

main.bin: main.c
	gcc -g main.c -o main.bin

clean:
	rm -f main.bin
