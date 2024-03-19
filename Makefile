all: main.bin

main.bin: main.c
	@echo "> Compilando..."
	gcc main.c -o main.bin
	@echo "> Ejecutando..."
	./main.bin

clean:
	rm -f main.bin
