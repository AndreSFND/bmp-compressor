all: concat

run: concat
	./programaTrab
	
concat: bitmap.o compressor.o main.o
	gcc -o programaTrab bitmap.o compressor.o main.o

bitmap.o: bitmap.h bitmap.c
	gcc -c bitmap.h bitmap.c

compressor.o: compressor.h compressor.c
	gcc -c compressor.h compressor.c
	
main.o: main.c
	gcc -c main.c