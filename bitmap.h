/**
 * O TAD eh responsavel por manipular as informacoes do arquivo bitmap
 * 
 * @author  Andre Santana Fernandes <11208537>
 * @author  Diogo Castanho Emidio <11297274>
 * @author  Leonardo Antonetti da Motta <11275338>
*/

#ifndef H_BITMAP

    #define H_BITMAP

        #include <stdio.h>
        #include <stdlib.h>
        #define WIDTH 8
        #define HEIGHT 8

        typedef struct bitmapfileheader BITMAP_FILEHEADER;
        typedef struct bitmapinfoheader BITMAP_INFOHEADER;
        typedef struct bitmaprgb BITMAP_RGB;

        void iniciaHeader(BITMAP_FILEHEADER** fileHeader);
        void liberaHeader(BITMAP_FILEHEADER** fileHeader);
        void leituraHeader(FILE *file, BITMAP_FILEHEADER *fileHeader);
        void escreveHeader(FILE *file, BITMAP_FILEHEADER *fileHeader);

        void iniciaInfoHeader(BITMAP_INFOHEADER** infoHeader);
        void liberaInfoHeader(BITMAP_INFOHEADER** infoHeader);
        void leituraInfoHeader(FILE *file, BITMAP_INFOHEADER *infoHeader);
        void escreveInfoHeader(FILE *file, BITMAP_INFOHEADER *infoHeader);

        void iniciaRGB(BITMAP_RGB** RGB);
        void liberaRGB(BITMAP_RGB** RGB);
        void leituraRGB(FILE *F, BITMAP_RGB *RGB);
        void escreveRGB(FILE *F, BITMAP_RGB *RGB);

        int bitmapWidth(BITMAP_INFOHEADER *infoHeader);
        int bitmapHeight(BITMAP_INFOHEADER *infoHeader);

        unsigned char getBitmapR(BITMAP_RGB *RGB, int i, int j);
        unsigned char getBitmapG(BITMAP_RGB *RGB, int i, int j);
        unsigned char getBitmapB(BITMAP_RGB *RGB, int i, int j);

        void setBitmapR(BITMAP_RGB *RGB, int i, int j, unsigned char value);
        void setBitmapG(BITMAP_RGB *RGB, int i, int j, unsigned char value);
        void setBitmapB(BITMAP_RGB *RGB, int i, int j, unsigned char value);

        int getBitCount(BITMAP_INFOHEADER *infoHeader);

#endif