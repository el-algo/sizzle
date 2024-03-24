all: main.bin

main.bin: main.c
	@echo "> Compilando..."
	gcc -g main.c -o main.bin
	@echo "> Ejecutando..."
	./main.bin program_a.siz

clean:
	rm -f main.bin
