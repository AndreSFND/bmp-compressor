all: concat

run: concat
	./programaTrab
	
concat: lista.o bitmap.o compressor.o main.o
	gcc -o programaTrab lista.o bitmap.o compressor.o main.o -lm

lista.o: lista.h lista.c
	gcc -c lista.h lista.c

bitmap.o: bitmap.h bitmap.c
	gcc -c bitmap.h bitmap.c

compressor.o: compressor.h compressor.c
	gcc -c compressor.h compressor.c
	
main.o: main.c
	gcc -c main.c