/**
 * O TAD eh responsavel por manipular as informacoes ao longo do processo de compressao do arquivo
 * 
 * @author Andre Santana Fernandes <11208537>
*/

#include "compressor.h"

/**
 * A estrutura armazena os dados de luminancia e crominancia de uma imagem
*/
struct compressorycbcr {       

    int **Y;
    int **Cr;
    int **Cb;

    int proporcaoR;
    int proporcaoB;

};

/**
 * Inicializa uma estrutura YCbCr
 * 
 * @param COMPRESSOR_YCBCR** YCbCr endereco da estrutura a ser inicializada
 * @param BITMAP_INFOHEADER *infoHeader cabecalho de informacoes do arquivo bitmap
*/
void iniciaYCBCR(COMPRESSOR_YCBCR** YCbCr, BITMAP_INFOHEADER *infoHeader) {

    int Width = bitmapWidth(infoHeader);
    int Height = bitmapHeight(infoHeader);

    *YCbCr = malloc(sizeof(COMPRESSOR_YCBCR));

    (*YCbCr)->Y = malloc( sizeof(int*) * Height );
    for(int i=0; i<Height; i++)
        (*YCbCr)->Y[i] = malloc( sizeof(int) * Width );

    (*YCbCr)->Cr = malloc( sizeof(int*) * Height );
    for(int i=0; i<Height; i++)
        (*YCbCr)->Cr[i] = malloc( sizeof(int) * Width );

    (*YCbCr)->Cb = malloc( sizeof(int*)* Height );
    for(int i=0; i<Height; i++)
        (*YCbCr)->Cb[i] = malloc( sizeof(int) * Width );

    (*YCbCr)->proporcaoR = 4;
    (*YCbCr)->proporcaoB = 4;

}

/**
 * Libera uma estrutura YCbCr
 * 
 * @param COMPRESSOR_YCBCR** YCbCr endereco da estrutura a ser inicializada
 * @param BITMAP_INFOHEADER *infoHeader cabecalho de informacoes do arquivo bitmap
*/
void liberaYCBCR(COMPRESSOR_YCBCR** YCbCr, BITMAP_INFOHEADER *infoHeader) {

    int Width = bitmapWidth(infoHeader);
    int Height = bitmapHeight(infoHeader);

    for(int i=0; i<Height; i++) {

        free((*YCbCr)->Y[i]);
        free((*YCbCr)->Cr[i]);
        free((*YCbCr)->Cb[i]);

    }
        
    free((*YCbCr)->Y);
    free((*YCbCr)->Cr);        
    free((*YCbCr)->Cb);

    free(*YCbCr);

}

/**
 * Converte as cores de um arquivo do formato RGB para YCbCr
 * 
 * @param BITMAP_RGB *RGB estrutura de dados das cores no formato RGB
 * @param COMPRESSOR_YCBCR *YCbCr estrutura de dados das cores no formato YCbCr
 * @param BITMAP_INFOHEADER *infoHeader cabecalho de informacoes da imagem
*/
void RGBparaYCBCR(BITMAP_RGB *RGB, COMPRESSOR_YCBCR *YCbCr, BITMAP_INFOHEADER *infoHeader) {

    int Width = bitmapWidth(infoHeader);
    int Height = bitmapHeight(infoHeader);

    for(int i=0; i< Height; i++) {

        for(int j=0; j< Width; j++) {

            int R = getBitmapR(RGB, i, j);
            int G = getBitmapG(RGB, i, j);
            int B = getBitmapB(RGB, i, j);

            int Y = 0.299*R + 0.587*G + 0.114*B;
            int Cb = 0.713*R - 0.713*Y;
            int Cr = 0.564*B - 0.564*Y;

            YCbCr->Y[i][j] = Y;
            YCbCr->Cb[i][j] = Cb;
            YCbCr->Cr[i][j] = Cr;
            
        }

    }

}

/**
 * Converte as cores de YCbCr para RGB
 * 
 * @param BITMAP_RGB *RGB estrutura de dados das cores no formato RGB
 * @param COMPRESSOR_YCBCR *YCbCr estrutura de dados das cores no formato YCbCr
 * @param BITMAP_INFOHEADER *infoHeader cabecalho de informacoes da imagem
*/
void YCBCRparaRGB(BITMAP_RGB *RGB, COMPRESSOR_YCBCR *YCbCr, BITMAP_INFOHEADER *infoHeader) {

    int Width = bitmapWidth(infoHeader);
    int Height = bitmapHeight(infoHeader);

    for(int i=0; i<Height; i++) {

        for(int j=0; j<Width; j++) {

            float proporcaoB = YCbCr->proporcaoB / 4.0;
            float proporcaoR = YCbCr->proporcaoR / 4.0;

            int Y = YCbCr->Y[i][j];
            // int Cb = YCbCr->Cb[i][(int)(j*proporcaoB)];
            // int Cr = YCbCr->Cr[i][(int)(j*proporcaoR)];

            int Cb = j % 2 == 0 ? YCbCr->Cb[i][(int)(j*proporcaoB)] : 0;
            int Cr = j % 2 == 0 ? YCbCr->Cr[i][(int)(j*proporcaoR)] : 0;

            int R = (Cb + 0.713*Y) / 0.713;
            int B = (Cr + 0.564*Y) / 0.564;
            int G = (Y - 0.299*R - 0.114*B) / 0.587;

            setBitmapR(RGB, i, j, (unsigned char) R);
            setBitmapG(RGB, i, j, (unsigned char) G);
            setBitmapB(RGB, i, j, (unsigned char) B);
            
        }

    }

}

/**
 * Realiza o processo de downsampling na proporcao 4:2:2, pois eh a que menos perde qualidade
 * 
 * @param COMPRESSOR_YCBCR *YCbCr estrutura de dados das cores no formato YCbCr
 * @param BITMAP_INFOHEADER *infoHeader cabecalho de informacoes da imagem
*/
void DownSampling(COMPRESSOR_YCBCR **YCbCr, BITMAP_INFOHEADER *infoHeader) {

    int proporcaoB = 2;
    int proporcaoR = 2;

    int Width = bitmapWidth(infoHeader);
    int Height = bitmapHeight(infoHeader);

    int resizedWidthB = ((int)(Width * ((*YCbCr)->proporcaoB / 4.0)));
    int resizedHeightB = Height;

    int resizedWidthR = ((int)(Width * ((*YCbCr)->proporcaoR / 4.0)));
    int resizedHeightR = Height;

    int **dsCb, **dsCr;

    // Aloca memoria para a nova matriz Cb e realiza o downsamlping em Cb
    dsCb = malloc( sizeof(int*) * resizedHeightB );
    for(int y=0; y<resizedHeightB; y++) {

        dsCb[y] = malloc( sizeof(int) * resizedWidthB );

        for (int x=0; x<resizedWidthB; x++) {

            dsCb[y][x] = (
                (*YCbCr)->Cb[y][x*2]
                + (*YCbCr)->Cb[y][(x*2)+1]
                +1
            ) / 2;
                
        }

    }
        

    // Aloca memoria para a nova matriz Cr e realiza o downsamlping em Cr
    dsCr = malloc( sizeof(int*) * resizedHeightR );
    for(int y=0; y<resizedHeightR; y++) {

        dsCr[y] = malloc( sizeof(int) * resizedWidthR );

        for (int x=0; x<resizedWidthR; x++) {

            dsCr[y][x] = ( 
                (*YCbCr)->Cr[y][x*2] 
                + (*YCbCr)->Cr[y][(x*2)+1] 
                +1
            ) / 2;

        }

    }

    // Libera a matriz Cb
    for(int i=0; i<Height; i++)
        free((*YCbCr)->Cb[i]);
    free((*YCbCr)->Cb);

    // Libera a matriz Cr
    for(int i=0; i<Height; i++)
        free((*YCbCr)->Cr[i]);
    free((*YCbCr)->Cr);

    // Atribui as novas matrizes a Cb e Cr
    (*YCbCr)->Cb = dsCb;
    (*YCbCr)->Cr = dsCr;

    // Atualiza as proporcoes de Cb e Cr
    (*YCbCr)->proporcaoB = proporcaoB;
    (*YCbCr)->proporcaoR = proporcaoR;

}

void DCT() {}
void Quantizacao() {}
void CodificacaoEntropia() {}
