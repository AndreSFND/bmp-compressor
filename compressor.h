/**
 * O TAD eh responsavel por manipular as informacoes ao longo do processo de compressao do arquivo
 * 
 * @author Andre Santana Fernandes <11208537>
*/

#ifndef H_COMPRESSOR

    #define H_COMPRESSOR

        #include <stdio.h>
        #include <stdlib.h>
        #include <math.h>
        #include "bitmap.h"
        #include "lista.h"
        #define PI 3.141592654

        typedef struct compressorycbcr COMPRESSOR_YCBCR;
        typedef struct compressormatriz COMPRESSOR_MATRIZ;
        typedef struct compressorlistas COMPRESSOR_LISTAS;

        void iniciaListas(COMPRESSOR_LISTAS **Listas);
        void liberaListas(COMPRESSOR_LISTAS **Listas);

        void iniciaMatriz(COMPRESSOR_MATRIZ **Matriz, int length);
        void liberaMatriz(COMPRESSOR_MATRIZ **Matriz, int length);

        int getMatrizValue(COMPRESSOR_MATRIZ *Matriz, int which, int index);
        void setMatrizValue(COMPRESSOR_MATRIZ *Matriz, int which, int index, int value);

        void iniciaYCBCR(COMPRESSOR_YCBCR **YCrCb);
        void liberaYCBCR(COMPRESSOR_YCBCR **YCbCr);

        void RGBparaYCBCR(BITMAP_RGB *RGB, COMPRESSOR_YCBCR *YCrCb);
        void YCBCRparaRGB(BITMAP_RGB *RGB, COMPRESSOR_YCBCR *YCrCb);

        void DownSampling(COMPRESSOR_YCBCR **YCrCb);

        void transformadaDCT(COMPRESSOR_YCBCR *frequencias, COMPRESSOR_YCBCR *YCbCr);
        void inversaDCT(COMPRESSOR_YCBCR *frequencias, COMPRESSOR_YCBCR *YCbCr);

        void Quantizacao(COMPRESSOR_YCBCR *quantizada, COMPRESSOR_YCBCR *frequencias);
        void inversaQuantizada(COMPRESSOR_YCBCR *quantizada, COMPRESSOR_YCBCR *frequencias);

        void Vetorizacao(COMPRESSOR_YCBCR *quantizada, COMPRESSOR_MATRIZ *vetorizados);
        void inversaVetorizacao(COMPRESSOR_YCBCR *quantizada, COMPRESSOR_MATRIZ *vetorizados);

        void RunLength(COMPRESSOR_MATRIZ *vetorizados, COMPRESSOR_LISTAS *Listas);
        void Codificacao();

        void teste(unsigned char testBlockA[8][8]);

#endif