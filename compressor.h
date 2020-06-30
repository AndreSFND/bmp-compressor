/**
 * O TAD eh responsavel por manipular as informacoes ao longo do processo de compressao do arquivo
 * 
 * @author Andre Santana Fernandes <11208537>
*/

#ifndef H_COMPRESSOR

    #define H_COMPRESSOR

        #include <stdio.h>
        #include <stdlib.h>
        #include "bitmap.h"

        typedef struct compressorycbcr COMPRESSOR_YCBCR;

        void iniciaYCBCR(COMPRESSOR_YCBCR **YCrCb, BITMAP_INFOHEADER *infoHeader);
        void liberaYCBCR(COMPRESSOR_YCBCR** YCbCr, BITMAP_INFOHEADER *infoHeader);

        void RGBparaYCBCR(BITMAP_RGB *RGB, COMPRESSOR_YCBCR *YCrCb, BITMAP_INFOHEADER *infoHeader);
        void YCBCRparaRGB(BITMAP_RGB *RGB, COMPRESSOR_YCBCR *YCrCb, BITMAP_INFOHEADER *infoHeader);

        void DownSampling(COMPRESSOR_YCBCR **YCrCb, BITMAP_INFOHEADER *infoHeader);
        void DCT();
        void Quantizacao();
        void CodificacaoEntropia();

#endif