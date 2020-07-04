/**
 * O TAD eh responsavel por manipular as informacoes ao longo do processo de compressao do arquivo
 * 
 * @author  Andre Santana Fernandes <11208537>
 * @author  Diogo Castanho Emidio <11297274>
 * @author  Leonardo Antonetti da Motta <11275338>
*/

#include "compressor.h"

/**
 * A estrutura armazena os dados de luminancia e crominancia de uma imagem
*/
struct compressorycbcr {       

    float **Y;
    float **Cr;
    float **Cb;

    int proporcaoR;
    int proporcaoB;

};

/**
 * A estrutura armazena tres vetores de dados das data units
*/
struct compressormatriz {

    float *vetorY;
    float *vetorCb;
    float *vetorCr;

    int length;

};

/**
 * A estrutura armazena tres listas de dados das data units
*/
struct compressorlistas {

    LISTA *listaY;
    LISTA *listaCr;
    LISTA *listaCb;

};

int categoriaCoeficiente(float coeficiente);
char* prefixoDC(int categoria);
char* prefixoAC(int zeros, int categoria);
char* sufixoDCAC(int categoria, int valor);
char *strrev(char *str);
void converterDeciBin(int deci, char *bin, int tamanho);
void converterBinDeci(char *bin, int *deci);
void inverteSufixoDCAC(char *sufixo, int categoria, int *valor);
int invertePrefixoDC(char *prefixo);
void invertePrefixoAC(char *prefixo, int *zeros, int *categoria);
void converteDC(int categoria, int DC, int *valor);
void converteAC(int categoria, int AC, int *valor, int *zeros);

void iniciaListas(COMPRESSOR_LISTAS **Listas) {

    *Listas = malloc(sizeof(COMPRESSOR_LISTAS));

    (*Listas)->listaY = Lista_Criar();
    (*Listas)->listaCr = Lista_Criar();
    (*Listas)->listaCb = Lista_Criar();

}

void liberaListas(COMPRESSOR_LISTAS **Listas) {

    Lista_Apagar( &((*Listas)->listaY) );
    Lista_Apagar( &((*Listas)->listaCr) );
    Lista_Apagar( &((*Listas)->listaCb) );

    free(*Listas);

}

void iniciaMatriz(COMPRESSOR_MATRIZ **Matriz, int length) {

    *Matriz = malloc(sizeof(COMPRESSOR_MATRIZ));

    (*Matriz)->vetorY = malloc( sizeof(int) * length );
    (*Matriz)->vetorCb = malloc( sizeof(int) * length );
    (*Matriz)->vetorCr = malloc( sizeof(int) * length );

}

void liberaMatriz(COMPRESSOR_MATRIZ **Matriz, int length) {

    free((*Matriz)->vetorY);
    free((*Matriz)->vetorCb);
    free((*Matriz)->vetorCr);

    free(*Matriz);

}

int getMatrizValue(COMPRESSOR_MATRIZ *Matriz, int which, int index) {

    int value;

    switch(which) {

        case 0: { value = Matriz->vetorY[index]; break; }
        case 1: { value = Matriz->vetorCb[index]; break; }
        case 2: { value = Matriz->vetorCr[index]; break; }

    }

    return value;

}

void setMatrizValue(COMPRESSOR_MATRIZ *Matriz, int which, int index, int value) {

    switch(which) {

        case 0: { Matriz->vetorY[index] = value; break; }
        case 1: { Matriz->vetorCb[index] = value; break; }
        case 2: { Matriz->vetorCr[index] = value; break; }

    }

}

/**
 * Inicializa uma estrutura YCbCr 8x8
 * 
 * @param COMPRESSOR_YCBCR** YCbCr endereco da estrutura a ser inicializada
*/
void iniciaYCBCR(COMPRESSOR_YCBCR **YCbCr) {

    *YCbCr = malloc(sizeof(COMPRESSOR_YCBCR));

    (*YCbCr)->Y = malloc( sizeof(int*) * HEIGHT );
    for(int i=0; i<HEIGHT; i++)
        (*YCbCr)->Y[i] = malloc( sizeof(int) * WIDTH );

    (*YCbCr)->Cr = malloc( sizeof(int*) * HEIGHT );
    for(int i=0; i<HEIGHT; i++)
        (*YCbCr)->Cr[i] = malloc( sizeof(int) * WIDTH );

    (*YCbCr)->Cb = malloc( sizeof(int*)* HEIGHT );
    for(int i=0; i<HEIGHT; i++)
        (*YCbCr)->Cb[i] = malloc( sizeof(int) * WIDTH );

    (*YCbCr)->proporcaoR = 4;
    (*YCbCr)->proporcaoB = 4;

}

/**
 * Libera uma estrutura YCbCr 8x8
 * 
 * @param COMPRESSOR_YCBCR** YCbCr endereco da estrutura a ser inicializada
*/
void liberaYCBCR(COMPRESSOR_YCBCR **YCbCr) {

    for(int i=0; i<HEIGHT; i++) {

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
*/
void RGBparaYCBCR(BITMAP_RGB *RGB, COMPRESSOR_YCBCR *YCbCr) {

    for(int i=0; i<HEIGHT; i++) {

        for(int j=0; j<WIDTH; j++) {

            int R = getBitmapR(RGB, i, j)-128;
            int G = getBitmapG(RGB, i, j)-128;
            int B = getBitmapB(RGB, i, j)-128;

            float Y = 0.299*R + 0.587*G + 0.114*B;
            float Cb = 0.713*R - 0.713*Y;
            float Cr = 0.564*B - 0.564*Y;

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
*/
void YCBCRparaRGB(BITMAP_RGB *RGB, COMPRESSOR_YCBCR *YCbCr) {

    for(int i=0; i<HEIGHT; i++) {

        for(int j=0; j<WIDTH; j++) {

            float proporcaoB = YCbCr->proporcaoB / 4.0;
            float proporcaoR = YCbCr->proporcaoR / 4.0;

            float Y = YCbCr->Y[i][j];
            float Cb = YCbCr->Cb[i][(int)(j*proporcaoB)];
            float Cr = YCbCr->Cr[i][(int)(j*proporcaoR)];

            int R = (Cb + 0.713*Y) / 0.713;
            int B = (Cr + 0.564*Y) / 0.564;
            int G = (Y - 0.299*R - 0.114*B) / 0.587;

            setBitmapR(RGB, i, j, (unsigned char) R+128);
            setBitmapG(RGB, i, j, (unsigned char) G+128);
            setBitmapB(RGB, i, j, (unsigned char) B+128);
            
        }

    }

}

/**
 * Realiza o processo de downsampling na proporcao 4:2:2, pois eh a que menos perde qualidade
 * 
 * @param COMPRESSOR_YCBCR *YCbCr estrutura de dados das cores no formato YCbCr
*/
void DownSampling(COMPRESSOR_YCBCR **YCbCr) {

    int proporcaoB = 2;
    int proporcaoR = 2;

    int resizedWidthB = (int) (WIDTH * ( proporcaoB / 4.0) );
    int resizedWidthR = (int) (WIDTH * ( proporcaoR / 4.0) );

    float **dsCb, **dsCr;

    // Aloca memoria para a nova matriz Cb e realiza o downsampling em Cb
    dsCb = malloc( sizeof(float*) * HEIGHT );
    for(int y=0; y<HEIGHT; y++) {

        dsCb[y] = malloc( sizeof(float) * resizedWidthB );

        for (int x=0; x<resizedWidthB; x++) {

            dsCb[y][x] = (
                (*YCbCr)->Cb[y][x*2]
                + (*YCbCr)->Cb[y][(x*2)+1]
                + 1
            ) / 2;
                
        }

    }        

    // Aloca memoria para a nova matriz Cr e realiza o downsamlping em Cr
    dsCr = malloc( sizeof(float*) * HEIGHT );
    for(int y=0; y<HEIGHT; y++) {

        dsCr[y] = malloc( sizeof(float) * resizedWidthR );

        for (int x=0; x<resizedWidthR; x++) {

            dsCr[y][x] = ( 
                (*YCbCr)->Cr[y][x*2] 
                + (*YCbCr)->Cr[y][(x*2)+1] 
                + 1
            ) / 2;

        }

    }

    // Libera a matriz Cb
    for(int i=0; i<HEIGHT; i++)
        free((*YCbCr)->Cb[i]);
    free((*YCbCr)->Cb);

    // Libera a matriz Cr
    for(int i=0; i<HEIGHT; i++)
        free((*YCbCr)->Cr[i]);
    free((*YCbCr)->Cr);

    // Atribui as novas matrizes a Cb e Cr
    (*YCbCr)->Cb = dsCb;
    (*YCbCr)->Cr = dsCr;

    // Atualiza as proporcoes de Cb e Cr
    (*YCbCr)->proporcaoB = proporcaoB;
    (*YCbCr)->proporcaoR = proporcaoR;

}

/**
 * Calcula a matriz de frequencias
 * 
 * @param COMPRESSOR_YCBCR *frequencias estrutura de dados da matriz de frequencias
 * @param COMPRESSOR_YCBCR *YCbCr estrutura de dados das cores no formato YCbCr
*/
void transformadaDCT(COMPRESSOR_YCBCR *frequencias, COMPRESSOR_YCBCR *YCbCr) {

    float proporcaoB = YCbCr->proporcaoB / 4.0;
    float proporcaoR = YCbCr->proporcaoR / 4.0;

    frequencias->proporcaoB = YCbCr->proporcaoB;
    frequencias->proporcaoR = YCbCr->proporcaoR;

    int resizedWidthB = (int) (WIDTH * proporcaoB );
    int resizedWidthR = (int) (WIDTH * proporcaoR );

    // Monta a matriz de frequencias
    for(int v=0; v<HEIGHT; v++) {

        for(int u=0; u<WIDTH; u++) {

            float uAlpha = ( u == 0 ? 1.0/sqrt(2.0) : 1.0 );
            float vAlpha = ( v == 0 ? 1.0/sqrt(2.0) : 1.0 );

            float sumY = 0;

            for(int x=0; x<WIDTH; x++)
                for(int y=0; y<HEIGHT; y++)
                    sumY += YCbCr->Y[x][y] * cos( ( (2.0*x + 1.0) * u * PI ) / 16.0 ) * cos( ( (2.0*y + 1.0) * v * PI ) / 16.0 );

            float frequenciaY = (uAlpha / 2.0) * (vAlpha / 2.0) * sumY;
            frequencias->Y[v][u] = frequenciaY;

        }

        for(int u=0; u<resizedWidthB; u++) {

            float uAlpha = ( u == 0 ? 1.0/sqrt(2.0) : 1.0 );
            float vAlpha = ( v == 0 ? 1.0/sqrt(2.0) : 1.0 );

            float sumCb = 0;

            for(int x=0; x<WIDTH; x++)
                for(int y=0; y<HEIGHT; y++)
                    sumCb += YCbCr->Cb[x][ (int)(y*proporcaoB) ] * cos( ( (2.0*x + 1.0) * u * PI ) / 16.0 ) * cos( ( (2.0*y + 1.0) * v * PI ) / 16.0 );

            float frequenciaCb = (uAlpha / 2.0) * (vAlpha / 2.0) * sumCb;
            frequencias->Cb[v][u] = frequenciaCb;

        }

        for(int u=0; u<resizedWidthR; u++) {

            float uAlpha = ( u == 0 ? 1.0/sqrt(2.0) : 1.0 );
            float vAlpha = ( v == 0 ? 1.0/sqrt(2.0) : 1.0 );

            float sumCr = 0;

            for(int x=0; x<WIDTH; x++)
                for(int y=0; y<HEIGHT; y++)
                    sumCr += YCbCr->Cr[x][ (int)(y*proporcaoR) ] * cos( ( (2.0*x + 1.0) * u * PI ) / 16.0 ) * cos( ( (2.0*y + 1.0) * v * PI ) / 16.0 );

            float frequenciaCr = (uAlpha / 2.0) * (vAlpha / 2.0) * sumCr;
            frequencias->Cr[v][u] = frequenciaCr;

        }

    }

}

void inversaDCT(COMPRESSOR_YCBCR *frequencias, COMPRESSOR_YCBCR *YCbCr) {

    // Monta a matriz YCbCr a partir da matriz de frequencias
    for(int y=0; y<HEIGHT; y++) {

        for(int x=0; x<WIDTH; x++) {

            float sumY = 0, sumCb = 0, sumCr = 0;

            for(int u=0; u<WIDTH; u++) {

                for(int v=0; v<HEIGHT; v++) {

                    float frequenciaY = frequencias->Y[u][v];
                    float frequenciaCb = frequencias->Cb[u][v];
                    float frequenciaCr = frequencias->Cr[u][v];

                    float uAlpha = ( u == 0 ? 1.0/sqrt(2.0) : 1.0 );
                    float vAlpha = ( v == 0 ? 1.0/sqrt(2.0) : 1.0 );

                    sumY += uAlpha * vAlpha * frequenciaY * cos( ( (2.0*x + 1.0) * u * PI ) / 16.0 ) * cos( ( (2.0*y + 1.0) * v * PI ) / 16.0 );
                    sumCb += uAlpha * vAlpha * frequenciaCb * cos( ( (2.0*x + 1.0) * u * PI ) / 16.0 ) * cos( ( (2.0*y + 1.0) * v * PI ) / 16.0 );
                    sumCr += uAlpha * vAlpha * frequenciaCr * cos( ( (2.0*x + 1.0) * u * PI ) / 16.0 ) * cos( ( (2.0*y + 1.0) * v * PI ) / 16.0 );

                }

            }

            float Y = sumY / 4;
            float Cb = sumCb / 4;
            float Cr = sumCr / 4;

            YCbCr->Y[y][x] = Y;
            YCbCr->Cb[y][x] = Cb;
            YCbCr->Cr[y][x] = Cr;

        }

    }

}

/**
 * Calcula a matriz quantizada
 * 
 * @param COMPRESSOR_YCBCR *frequencias estrutura de dados da matriz de frequencias
 * @param COMPRESSOR_YCBCR *quantizada estrutura de dados da matriz quantizada
*/
void Quantizacao(COMPRESSOR_YCBCR *quantizada, COMPRESSOR_YCBCR *frequencias, int fatorCompressao) {

    float proporcaoB = frequencias->proporcaoB / 4.0;
    float proporcaoR = frequencias->proporcaoR / 4.0;

    quantizada->proporcaoB = frequencias->proporcaoB;
    quantizada->proporcaoR = frequencias->proporcaoR;

    int resizedWidthB = (int) (WIDTH * proporcaoB );
    int resizedWidthR = (int) (WIDTH * proporcaoR );

    // Fator de compressao
    int k = fatorCompressao;

    // Matriz de quantizacao de luminancia
    int QLuminancia[8][8] = {

        { 9, 9, 12, 24, 50, 50, 50, 50 },
        { 9,  11,  13, 33, 50, 50, 50, 50},
        { 12, 13, 28, 50, 50, 50, 50, 50 },
        { 24, 33, 50, 50, 50, 50, 50, 50 },
        { 50, 50, 50, 50, 50, 50, 50, 50},
        { 50, 50, 50, 50, 50, 50, 50, 50},
        { 50, 50, 50, 50, 50, 50, 50, 50},
        { 50, 50, 50, 50, 50, 50, 50, 50},

    };

    // Matriz de quantizacao de crominancia
    int QCrominancia[8][8] = {

        { 8, 6, 5, 8, 12, 20, 26, 31 },
        { 6, 6, 7, 10, 13, 29, 30, 28 },
        { 7, 7, 8, 12, 20, 29, 35, 28 },
        { 7, 9, 11, 15, 26, 44, 40, 31 },
        { 9, 11, 19, 28, 34, 55, 52, 39 },
        { 12, 18, 28, 32, 41, 52, 27, 46 },
        { 25, 32, 39, 44, 52, 61, 60, 51 },
        { 36, 46, 48, 49, 56, 50, 52, 50 },

    };

    for(int i=0; i<HEIGHT; i++) {

        for(int j=0; j<WIDTH; j++) {

            quantizada->Y[i][j] = round( frequencias->Y[i][j] / ( k * QLuminancia[i][j] ) );

        }

        for(int j=0; j<resizedWidthB; j++) {

            quantizada->Cb[i][j] = round( frequencias->Cb[i][j] / ( k * QCrominancia[i][j] ) );

        }

        for(int j=0; j<resizedWidthR; j++) {

            quantizada->Cr[i][j] = round( frequencias->Cr[i][j] / ( k * QCrominancia[i][j] ) );

        }

    }

}

void inversaQuantizada(COMPRESSOR_YCBCR *quantizada, COMPRESSOR_YCBCR *frequencias, int fatorCompressao) {

    // Fator de compressao
    int k = fatorCompressao;

    // Matriz de quantizacao de luminancia
    int QLuminancia[8][8] = {

        { 9, 9, 12, 24, 50, 50, 50, 50 },
        { 9,  11,  13, 33, 50, 50, 50, 50},
        { 12, 13, 28, 50, 50, 50, 50, 50 },
        { 24, 33, 50, 50, 50, 50, 50, 50 },
        { 50, 50, 50, 50, 50, 50, 50, 50},
        { 50, 50, 50, 50, 50, 50, 50, 50},
        { 50, 50, 50, 50, 50, 50, 50, 50},
        { 50, 50, 50, 50, 50, 50, 50, 50},

    };

    // Matriz de quantizacao de crominancia
    int QCrominancia[8][8] = {

        { 8, 6, 5, 8, 12, 20, 26, 31 },
        { 6, 6, 7, 10, 13, 29, 30, 28 },
        { 7, 7, 8, 12, 20, 29, 35, 28 },
        { 7, 9, 11, 15, 26, 44, 40, 31 },
        { 9, 11, 19, 28, 34, 55, 52, 39 },
        { 12, 18, 28, 32, 41, 52, 27, 46 },
        { 25, 32, 39, 44, 52, 61, 60, 51 },
        { 36, 46, 48, 49, 56, 50, 52, 50 },

    };

    for(int i=0; i<HEIGHT; i++) {

        for(int j=0; j<WIDTH; j++) {

            frequencias->Y[i][j] = quantizada->Y[i][j] * k * QLuminancia[i][j];
            frequencias->Cb[i][j] = quantizada->Cb[i][j] * k * QCrominancia[i][j];
            frequencias->Cr[i][j] = quantizada->Cr[i][j] * k * QCrominancia[i][j];

        }

    }

}

/**
 * Monta os vetores ordenados em zig zag (vetorizacao)
 * 
 * @param COMPRESSOR_YCBCR *quantizada estrutura de dados da matriz quantizada
 * @param COMPRESSOR_MATRIZ *vetorizados estrutura de dados da matriz vetorizada
*/
void Vetorizacao(COMPRESSOR_YCBCR *quantizada, COMPRESSOR_MATRIZ *vetorizados) {

    float proporcaoB = quantizada->proporcaoB / 4.0;
    float proporcaoR = quantizada->proporcaoR / 4.0;

    int resizedWidthB = (int) (WIDTH * proporcaoB );
    int resizedWidthR = (int) (WIDTH * proporcaoR );

    int x = 0, y = 0;

    for(int i=0; i<HEIGHT*WIDTH; i++) {

        vetorizados->vetorY[i] = quantizada->Y[y][x];

        // Direita
        if((y == 0 || y == HEIGHT-1) && x < WIDTH-1  && x % 2 == 0) {
        
            x++;
        
        } 
        // Baixo
        else if((x == 0 || x == WIDTH-1) && y < HEIGHT-1 && y % 2 != 0) {

            y++;

        }
        // Diagonal para baixo
        else if( (x + y) % 2 != 0 && x > 0 && y < HEIGHT-1 ) {

            x--;
            y++;

        }
        // Diagonal para cima
        else if( (x + y) % 2 == 0 && y > 0 && x < WIDTH-1 ) {

            x++;
            y--;

        }

    }

    x = 0;
    y = 0;

    for(int i=0; i<HEIGHT*resizedWidthB; i++) {

        vetorizados->vetorCb[i] = quantizada->Cb[y][x];

        // Direita
        if((y == 0 || y == HEIGHT-1) && x < resizedWidthB-1  && x % 2 == 0) {
        
            x++;
        
        } 
        // Baixo
        else if((x == 0 || x == resizedWidthB-1) && y < HEIGHT-1 && y % 2 != 0) {

            y++;

        }
        // Diagonal para baixo
        else if( (x + y) % 2 != 0 && x > 0 && y < HEIGHT-1 ) {

            x--;
            y++;

        }
        // Diagonal para cima
        else if( (x + y) % 2 == 0 && y > 0 && x < resizedWidthB-1 ) {

            x++;
            y--;

        }

    }

    x = 0;
    y = 0;

    for(int i=0; i<HEIGHT*resizedWidthR; i++) {

        vetorizados->vetorCr[i] = quantizada->Cr[y][x];

        // Direita
        if((y == 0 || y == HEIGHT-1) && x < resizedWidthR-1  && x % 2 == 0) {
        
            x++;
        
        } 
        // Baixo
        else if((x == 0 || x == resizedWidthR-1) && y < HEIGHT-1 && y % 2 != 0) {

            y++;

        }
        // Diagonal para baixo
        else if( (x + y) % 2 != 0 && x > 0 && y < HEIGHT-1 ) {

            x--;
            y++;

        }
        // Diagonal para cima
        else if( (x + y) % 2 == 0 && y > 0 && x < resizedWidthR-1 ) {

            x++;
            y--;

        }

    }

}

void inversaVetorizacao(COMPRESSOR_YCBCR *quantizada, COMPRESSOR_MATRIZ *vetorizados) {

    int x = 0, y = 0;

    for(int i=0; i<WIDTH*HEIGHT; i++) {

        quantizada->Y[y][x] = vetorizados->vetorY[i];
        quantizada->Cb[y][x] = vetorizados->vetorCb[i];
        quantizada->Cr[y][x] = vetorizados->vetorCr[i];

        // Direita
        if((y == 0 || y == HEIGHT-1) && x < WIDTH-1  && x % 2 == 0) {
        
            x++;
        
        } 
        // Baixo
        else if((x == 0 || x == WIDTH-1) && y < HEIGHT-1 && y % 2 != 0) {

            y++;

        }
        // Diagonal para baixo
        else if( (x + y) % 2 != 0 && x > 0 && y < HEIGHT-1 ) {

            x--;
            y++;

        }
        // Diagonal para cima
        else if( (x + y) % 2 == 0 && y > 0 && x < WIDTH-1 ) {

            x++;
            y--;

        }

    }

}

void RunLength(COMPRESSOR_MATRIZ *vetorizados, COMPRESSOR_LISTAS *codificadosAC) {

    int zerosY = 0,  zerosCb = 0, zerosCr = 0;

    for(int i=1; i<HEIGHT*WIDTH; i++) {

        if(vetorizados->vetorY[i] == 0) zerosY++;
        else {
            Lista_Inserir(codificadosAC->listaY, zerosY, vetorizados->vetorY[i], categoriaCoeficiente(vetorizados->vetorY[i]));
            zerosY = 0;
        }

        if(vetorizados->vetorCb[i] == 0) zerosCb++;
        else {
            Lista_Inserir(codificadosAC->listaCb, zerosCb, vetorizados->vetorCb[i], categoriaCoeficiente(vetorizados->vetorCb[i]));
            zerosCb = 0;
        }

        if(vetorizados->vetorCr[i] == 0) zerosCr++;
        else {
            Lista_Inserir(codificadosAC->listaCr, zerosCr, vetorizados->vetorCr[i], categoriaCoeficiente(vetorizados->vetorCr[i]));
            zerosCr = 0;
        }

    }

}

void inversaRunLength(COMPRESSOR_MATRIZ *vetorizados, COMPRESSOR_LISTAS *codificadosAC) {

    int zerosY = 0,  zerosCb = 0, zerosCr = 0;
    int valorY = -1,  valorCb = -1, valorCr = -1;
    int valorYFoiUtilizado = 1, valorCbFoiUtilizado = 1, valorCrFoiUtilizado = 1;

    NO* noY = Lista_No(codificadosAC->listaY, 0);
    NO* noCb = Lista_No(codificadosAC->listaCb, 0);
    NO* noCr = Lista_No(codificadosAC->listaCr, 0);

    if(noY != NULL) {
        
        valorY = Lista_No_Valor(noY);
        zerosY = Lista_No_Zeros(noY);
        valorYFoiUtilizado = 0;

        noY = Lista_No_Proximo(noY);
    
    } 
    if(noCb != NULL) {
        
        valorCb = Lista_No_Valor(noCb);
        zerosCb = Lista_No_Zeros(noCb);
        valorCbFoiUtilizado = 0;

        noCb = Lista_No_Proximo(noCb);
    
    }
    if(noCr != NULL) {
        
        valorCr = Lista_No_Valor(noCr);
        zerosCr = Lista_No_Zeros(noCr);
        valorCrFoiUtilizado = 0;

        noCr = Lista_No_Proximo(noCr);
    
    }

    for(int i=1; i<HEIGHT*WIDTH; i++) {

        if(zerosY > 0) {

            vetorizados->vetorY[i] = 0;
            zerosY--;

        } else if(!valorYFoiUtilizado) {

            vetorizados->vetorY[i] = valorY;
            valorYFoiUtilizado = 1;

            if(noY != NULL) {
        
                valorY = Lista_No_Valor(noY);
                zerosY = Lista_No_Zeros(noY);
                valorYFoiUtilizado = 0;

                noY = Lista_No_Proximo(noY);
            
            }

        } else {

            vetorizados->vetorY[i] = 0;

        }

        if(zerosCb > 0) {

            vetorizados->vetorCb[i] = 0;
            zerosCb--;

        } else if(!valorCbFoiUtilizado) {

            vetorizados->vetorCb[i] = valorCb;
            valorCbFoiUtilizado = 1;

            if(noCb != NULL) {
        
                valorCb = Lista_No_Valor(noCb);
                zerosCb = Lista_No_Zeros(noCb);
                valorCbFoiUtilizado = 0;

                noCb = Lista_No_Proximo(noCb);
            
            }

        } else {

            vetorizados->vetorCb[i] = 0;

        }

        if(zerosCr > 0) {

            vetorizados->vetorCr[i] = 0;
            zerosCr--;

        } else if(!valorCrFoiUtilizado) {

            vetorizados->vetorCr[i] = valorCr;
            valorCrFoiUtilizado = 1;

            if(noCr != NULL) {
        
                valorCr = Lista_No_Valor(noCr);
                zerosCr = Lista_No_Zeros(noCr);
                valorCrFoiUtilizado = 0;

                noCr = Lista_No_Proximo(noCr);
            
            }

        } else {

            vetorizados->vetorCr[i] = 0;

        }

    }

}

void codificacaoEstatistica(FILE *file, int DCY, int DCCb, int DCCr, COMPRESSOR_LISTAS *codificadosAC, int fatorCompressao) {

    unsigned char fator = (unsigned char) fatorCompressao;
    fwrite(&fator, sizeof(unsigned char), 1, file);

    int i, length;
    unsigned short int buffer = 0; // Ocupa 2 bytes de espaco de memoria*/
    unsigned char categoria;
    char *fim = "1010";

    // Calcula o codigo de DC em Y
    categoria = (unsigned char) categoriaCoeficiente(DCY);
    char *_prefixoDC = prefixoDC(categoriaCoeficiente(DCY));
    char *_sufixoDC = sufixoDCAC(categoriaCoeficiente(DCY), DCY);

    length = strlen(_prefixoDC) + strlen(_sufixoDC);

    char *codigo = calloc(length, sizeof(char));
    strcat(codigo, _prefixoDC);
    strcat(codigo, _sufixoDC);

    // Transfere o conteudo do bitstream para um byte
    buffer = 0;
    for (i = 0; i < length; i++) 
        buffer = (buffer << 1) | (codigo[i] == '1');

    fwrite(&categoria, sizeof(unsigned char), 1, file);
    fwrite(&buffer, sizeof(buffer), 1, file);
    free(codigo);

    // Calcula o codigo de DC em Cb
    categoria = (unsigned char) categoriaCoeficiente(DCCb);
    _prefixoDC = prefixoDC(categoriaCoeficiente(DCCb));
    _sufixoDC = sufixoDCAC(categoriaCoeficiente(DCCb), DCCb);

    length = strlen(_prefixoDC) + strlen(_sufixoDC);

    codigo = calloc(length, sizeof(char));
    strcat(codigo, _prefixoDC);
    strcat(codigo, _sufixoDC);

    // Transfere o conteudo do bitstream para um byte
    buffer = 0;
    for (i = 0; i < length; i++) 
        buffer = (buffer << 1) | (codigo[i] == '1');

    fwrite(&categoria, sizeof(unsigned char), 1, file);
    fwrite(&buffer, sizeof(buffer), 1, file);
    free(codigo);

    // Calcula o codigo de DC em Cr
    categoria = (unsigned char) categoriaCoeficiente(DCCr);
    _prefixoDC = prefixoDC(categoriaCoeficiente(DCCr));
    _sufixoDC = sufixoDCAC(categoriaCoeficiente(DCCr), DCCr);

    length = strlen(_prefixoDC) + strlen(_sufixoDC);

    codigo = calloc(length, sizeof(char));
    strcat(codigo, _prefixoDC);
    strcat(codigo, _sufixoDC);

    // Transfere o conteudo do bitstream para um byte
    buffer = 0;
    for (i = 0; i < length; i++) 
        buffer = (buffer << 1) | (codigo[i] == '1');

    fwrite(&categoria, sizeof(unsigned char), 1, file);
    fwrite(&buffer, sizeof(buffer), 1, file);
    free(codigo);

    NO *no = Lista_No(codificadosAC->listaY, 0);
    while (no != NULL) {

        categoria = (unsigned char) Lista_No_Categoria(no);
        char *_prefixoAC = prefixoAC(Lista_No_Zeros(no), Lista_No_Categoria(no));
        char *_sufixoAC = sufixoDCAC(Lista_No_Categoria(no), Lista_No_Valor(no));

        length = strlen(_prefixoAC) + strlen(_sufixoAC);

        char *codigoAC = calloc(length, sizeof(char));
        strcat(codigoAC, _prefixoAC);
        strcat(codigoAC, _sufixoAC);

        // Transfere o conteudo do bitstream para um byte
        buffer = 0;
        for (i = 0; i < length; i++) 
            buffer = (buffer << 1) | (codigoAC[i] == '1');

        fwrite(&categoria, sizeof(unsigned char), 1, file);
        fwrite(&buffer, sizeof(buffer), 1, file);
        free(codigoAC);

        no = Lista_No_Proximo(no);

    }

    categoria = '0';
    length = strlen(fim);

    // Transfere o conteudo do bitstream para um byte
    buffer = 0;
    for (i = 0; i < length; i++) 
        buffer = (buffer << 1) | (fim[i] == '1');

    fwrite(&categoria, sizeof(unsigned char), 1, file);
    fwrite(&buffer, sizeof(buffer), 1, file);

    no = Lista_No(codificadosAC->listaCb, 0);
    while (no != NULL) {

        categoria = (unsigned char) Lista_No_Categoria(no);
        char *_prefixoAC = prefixoAC(Lista_No_Zeros(no), Lista_No_Categoria(no));
        char *_sufixoAC = sufixoDCAC(Lista_No_Categoria(no), Lista_No_Valor(no));

        length = strlen(_prefixoAC) + strlen(_sufixoAC);

        char *codigoAC = calloc(length, sizeof(char));
        strcat(codigoAC, _prefixoAC);
        strcat(codigoAC, _sufixoAC);

        // Transfere o conteudo do bitstream para um byte
        buffer = 0;
        for (i = 0; i < length; i++) 
            buffer = (buffer << 1) | (codigoAC[i] == '1');

        fwrite(&categoria, sizeof(unsigned char), 1, file);
        fwrite(&buffer, sizeof(buffer), 1, file);
        free(codigoAC);

        no = Lista_No_Proximo(no);

    }

    categoria = '0';
    length = strlen(fim);

    // Transfere o conteudo do bitstream para um byte
    buffer = 0;
    for (i = 0; i < length; i++) 
        buffer = (buffer << 1) | (fim[i] == '1');

    fwrite(&categoria, sizeof(unsigned char), 1, file);
    fwrite(&buffer, sizeof(buffer), 1, file);

    no = Lista_No(codificadosAC->listaCr, 0);
    while (no != NULL) {

        categoria = (unsigned char) Lista_No_Categoria(no);
        char *_prefixoAC = prefixoAC(Lista_No_Zeros(no), Lista_No_Categoria(no));
        char *_sufixoAC = sufixoDCAC(Lista_No_Categoria(no), Lista_No_Valor(no));

        length = strlen(_prefixoAC) + strlen(_sufixoAC);

        char *codigoAC = calloc(length, sizeof(char));
        strcat(codigoAC, _prefixoAC);
        strcat(codigoAC, _sufixoAC);

        // Transfere o conteudo do bitstream para um byte
        buffer = 0;
        for (i = 0; i < length; i++) 
            buffer = (buffer << 1) | (codigoAC[i] == '1');

        fwrite(&categoria, sizeof(unsigned char), 1, file);
        fwrite(&buffer, sizeof(buffer), 1, file);
        free(codigoAC);

        no = Lista_No_Proximo(no);

    }

    categoria = '0';
    length = strlen(fim);

    // Transfere o conteudo do bitstream para um byte
    buffer = 0;
    for (i = 0; i < length; i++) 
        buffer = (buffer << 1) | (fim[i] == '1');

    fwrite(&categoria, sizeof(unsigned char), 1, file);
    fwrite(&buffer, sizeof(buffer), 1, file);

}

void leituraEstatistica(FILE *file, int *DCY, int *DCCb, int *DCCr, COMPRESSOR_LISTAS *codificadosAC, int *fatorCompressao) {

    unsigned char fator;
    fread(&fator, sizeof(unsigned char), 1, file);
    *fatorCompressao = (int) fator;

    unsigned char categoria;
    char *prefixo, *sufixo;
    int codigo, zeros, valor;

    fread(&categoria, sizeof(unsigned char), 1, file);
    fread(&codigo, sizeof(unsigned short int), 1, file);
    converteDC((int)categoria, codigo, DCY);

    fread(&categoria, sizeof(unsigned char), 1, file);
    fread(&codigo, sizeof(unsigned short int), 1, file);
    converteDC((int)categoria, codigo, DCCb);
    
    fread(&categoria, sizeof(unsigned char), 1, file);
    fread(&codigo, sizeof(unsigned short int), 1, file);
    converteDC((int)categoria, codigo, DCCr);

    codigo = 0;

    while( codigo != 10 ) {

        fread(&categoria, sizeof(unsigned char), 1, file);
        fread(&codigo, sizeof(unsigned short int), 1, file);

        if(codigo == 10) break;

        converteAC((int)categoria, codigo, &valor, &zeros);
        Lista_Inserir(codificadosAC->listaY, zeros, valor, categoria);

    }

    codigo = 0;

    while( codigo != 10 ) {

        fread(&categoria, sizeof(unsigned char), 1, file);
        fread(&codigo, sizeof(unsigned short int), 1, file);

        if(codigo == 10) break;

        converteAC((int)categoria, codigo, &valor, &zeros);
        Lista_Inserir(codificadosAC->listaCb, zeros, valor, categoria);

    }

    codigo = 0;

    while( codigo != 10 ) {

        fread(&categoria, sizeof(unsigned char), 1, file);
        fread(&codigo, sizeof(unsigned short int), 1, file);
        
        if(codigo == 10) break;

        converteAC((int)categoria, codigo, &valor, &zeros);
        Lista_Inserir(codificadosAC->listaCr, zeros, valor, categoria);

    }

}

// Funcoes auxiliares

int categoriaCoeficiente(float coeficiente) {

    int categoria = 0;

    while(true) {

        int range = pow(2, categoria)-1;

        if(abs(coeficiente) <= range)
            return categoria;

        categoria++;

    }

}

char* prefixoDC(int categoria) {

    char* prefixos[11] = {

        "010",
        "011",
        "100",
        "00",
        "101",
        "110",
        "1110",
        "11110",
        "111110",
        "1111110",
        "11111110"

    };

    if(categoria >= 0 && categoria <= 10)
        return prefixos[categoria];
    
    return "";

}

char* prefixoAC(int zeros, int categoria) {

    if(zeros == 0 && categoria == 0)
        return "1010";
    else if(zeros == 15 && categoria == 0)
        return "111111110111";

    char* prefixos[16][10] = {

        { "00", "01", "100", "1011", "11010", "111000", "1111000", "1111110110", "1111111110000010", "1111111110000011" },
        { "1100", "111001", "1111001", "111110110", "11111110110", "1111111110000100", "1111111110000101", "1111111110000110", "1111111110000111", "1111111110001000" },
        { "11011", "11111000", "1111110111", "1111111110001001", "1111111110001010", "1111111110001011", "1111111110001100", "1111111110001101", "1111111110001110", "1111111110001111" },
        { "111010", "111110111", "11111110111", "1111111110010000", "1111111110010001", "1111111110010010", "1111111110010011", "1111111110010100", "1111111110010101", "1111111110010110" },
        { "111011", "1111111000", "1111111110010111", "1111111110011000", "1111111110011001", "1111111110011010", "1111111110011011", "1111111110011100", "1111111110011101", "1111111110011110" },
        { "1111010", "1111111001", "1111111110011111", "1111111110100000", "1111111110100001", "1111111110100010", "1111111110100011", "1111111110100100", "1111111110100101", "1111111110100110" },
        { "1111011", "11111111000", "1111111110100111", "1111111110101000", "1111111110101001", "1111111110101010", "1111111110101011", "1111111110101100", "1111111110101101", "1111111110101110" },
        { "11111001", "11111111001", "1111111110101111", "1111111110110000", "1111111110110001", "1111111110110010", "1111111110110011", "1111111110110100", "1111111110110101", "1111111110110110" },
        { "11111010", "111111111000000", "1111111110110111", "1111111110111000", "1111111110111001", "1111111110111010", "1111111110111011", "1111111110111100", "1111111110111101", "1111111110111110" },
        { "111111000", "1111111110111111", "1111111111000000", "1111111111000001", "1111111111000010", "1111111111000011", "1111111111000100", "1111111111000101", "1111111111000110", "1111111111000111" },
        { "111111001", "1111111111001000", "1111111111001001", "1111111111001010", "1111111111001011", "1111111111001100", "1111111111001101", "1111111111001110", "1111111111001111", "1111111111010000" },
        { "111111010", "1111111111010001", "1111111111010010", "1111111111010011", "1111111111010100", "1111111111010101", "1111111111010110", "1111111111010111", "1111111111011000", "1111111111011001" },
        { "1111111010", "1111111111011010", "1111111111011011", "1111111111011100", "1111111111011101", "1111111111011110", "1111111111011111", "1111111111100000", "1111111111100001", "1111111111100010" },
        { "11111111010", "1111111111100011", "1111111111100100", "1111111111100101", "1111111111100110", "1111111111100111", "1111111111101000", "1111111111101001", "1111111111101010", "1111111111101011" },
        { "111111110110", "1111111111101100", "1111111111101101", "1111111111101110", "1111111111101111", "1111111111110000", "1111111111110001", "1111111111110010", "1111111111110011", "1111111111110100" },
        { "1111111111110101", "1111111111110110", "1111111111110111", "1111111111111000", "1111111111111001", "1111111111111010", "1111111111111011", "1111111111111100", "1111111111111101", "1111111111111110" },

    }; 

    if(zeros >= 0 && zeros <= 15 && categoria >= 0 && categoria <= 10)
        return prefixos[zeros][categoria-1];

    return "";

}

char* sufixoDCAC(int categoria, int valor) {

    if(!categoria)
        return "";

    char *sufixo = calloc(categoria, sizeof(char));

    if(valor == -1 || valor == 1) {
    
        if(valor == 1) sufixo[0] = '1';
        else sufixo[0] = '0';

        return sufixo;
    
    }

    int min, cont = 0;

    min = (pow(2, categoria) * -1) + 1;

    if(valor > 0)
        cont += min;

    for(min; min < valor; min++)
        cont++;

    converterDeciBin(cont, sufixo, categoria);

    return sufixo;

}

/**
 * Espelha uma string
 * 
 * @param char *str string a ser espelhada
 * @returns char* string espelhada
*/
char *strrev(char *str) {
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2) {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

void converterDeciBin(int deci, char *bin, int tamanho) {

    for(int i=0; i<tamanho; i++) {

        bin[i] = (deci % 2) + '0';
        deci /= 2;
    
    }

    bin = strrev(bin);

}

void converterBinDeci(char *bin, int *deci) {

    *deci = 0;

    char *inverso = strrev(bin);

    for(int i=0; i < strlen(inverso); i++)
        if(inverso[i] == '1')
            *deci += pow(2, i);

}

void inverteSufixoDCAC(char *sufixo, int categoria, int *valor) {

    if(!sufixo)
        *valor = -1;
    else {

        converterBinDeci(sufixo, valor);

        int count = *valor;
        int min = (pow(2, categoria) * -1) + 1;

        int v = min + count;
        if(v > (pow(2, categoria-1) * -1))
            v = v - min;

        *valor = v;

    }

}

int invertePrefixoDC(char *prefixo) {

    char* prefixos[11] = {

        "010",
        "011",
        "100",
        "00",
        "101",
        "110",
        "1110",
        "11110",
        "111110",
        "1111110",
        "11111110"

    };

    for(int i = 0; i < 12; i++) if(!strcmp(prefixo, prefixos[i]))
        return i;
        
    return -1;

}

void invertePrefixoAC(char *prefixo, int *zeros, int *categoria) {

    char* prefixos[16][10] = {

        { "00", "01", "100", "1011", "11010", "111000", "1111000", "1111110110", "1111111110000010", "1111111110000011" },
        { "1100", "111001", "1111001", "111110110", "11111110110", "1111111110000100", "1111111110000101", "1111111110000110", "1111111110000111", "1111111110001000" },
        { "11011", "11111000", "1111110111", "1111111110001001", "1111111110001010", "1111111110001011", "1111111110001100", "1111111110001101", "1111111110001110", "1111111110001111" },
        { "111010", "111110111", "11111110111", "1111111110010000", "1111111110010001", "1111111110010010", "1111111110010011", "1111111110010100", "1111111110010101", "1111111110010110" },
        { "111011", "1111111000", "1111111110010111", "1111111110011000", "1111111110011001", "1111111110011010", "1111111110011011", "1111111110011100", "1111111110011101", "1111111110011110" },
        { "1111010", "1111111001", "1111111110011111", "1111111110100000", "1111111110100001", "1111111110100010", "1111111110100011", "1111111110100100", "1111111110100101", "1111111110100110" },
        { "1111011", "11111111000", "1111111110100111", "1111111110101000", "1111111110101001", "1111111110101010", "1111111110101011", "1111111110101100", "1111111110101101", "1111111110101110" },
        { "11111001", "11111111001", "1111111110101111", "1111111110110000", "1111111110110001", "1111111110110010", "1111111110110011", "1111111110110100", "1111111110110101", "1111111110110110" },
        { "11111010", "111111111000000", "1111111110110111", "1111111110111000", "1111111110111001", "1111111110111010", "1111111110111011", "1111111110111100", "1111111110111101", "1111111110111110" },
        { "111111000", "1111111110111111", "1111111111000000", "1111111111000001", "1111111111000010", "1111111111000011", "1111111111000100", "1111111111000101", "1111111111000110", "1111111111000111" },
        { "111111001", "1111111111001000", "1111111111001001", "1111111111001010", "1111111111001011", "1111111111001100", "1111111111001101", "1111111111001110", "1111111111001111", "1111111111010000" },
        { "111111010", "1111111111010001", "1111111111010010", "1111111111010011", "1111111111010100", "1111111111010101", "1111111111010110", "1111111111010111", "1111111111011000", "1111111111011001" },
        { "1111111010", "1111111111011010", "1111111111011011", "1111111111011100", "1111111111011101", "1111111111011110", "1111111111011111", "1111111111100000", "1111111111100001", "1111111111100010" },
        { "11111111010", "1111111111100011", "1111111111100100", "1111111111100101", "1111111111100110", "1111111111100111", "1111111111101000", "1111111111101001", "1111111111101010", "1111111111101011" },
        { "111111110110", "1111111111101100", "1111111111101101", "1111111111101110", "1111111111101111", "1111111111110000", "1111111111110001", "1111111111110010", "1111111111110011", "1111111111110100" },
        { "1111111111110101", "1111111111110110", "1111111111110111", "1111111111111000", "1111111111111001", "1111111111111010", "1111111111111011", "1111111111111100", "1111111111111101", "1111111111111110" },

    };

    if(!strcmp(prefixo, "1010")) {

        *zeros = 0;
        *categoria = 0;
    
    } else if(!strcmp(prefixo, "111111110111")) {

        *zeros = 15;
        *categoria = 0;

    } else {

        for(int i = 0; i < 16; i++) {

            for(int j = 0; j < 10; j++) {

                if(!strcmp(prefixos[i][j], prefixo)) {

                    *zeros = i;
                    *categoria = j-1;

                }

            }
            
        }

    }

}

void converteDC(int categoria, int DC, int *valor) {

    char* codigo = calloc((sizeof(int)*8), sizeof(char));
    converterDeciBin(DC, codigo, (sizeof(int)*8));

    char* prefixo = calloc( (sizeof(int)*8) - categoria, sizeof(char) );
    char* _sufixo = calloc( categoria, sizeof(char) );

    strncpy(prefixo, codigo, (sizeof(int)*8) - categoria);
    strncpy(_sufixo, &codigo[strlen(codigo) - categoria], categoria);

    char *_prefixo;
    int length;

    for(int i=0; i<sizeof(int)*8; i++) if(prefixo[i] == '1' || i >= strlen(prefixo)-2) {

        length = (sizeof(int)*8) - i;

        _prefixo = calloc(length, sizeof(char));
        strcpy(_prefixo, &prefixo[i]);

        break;

    }

    inverteSufixoDCAC(_sufixo, categoria, valor);

    free(prefixo); free(_prefixo); free(_sufixo); free(codigo);

}

void converteAC(int categoria, int AC, int *valor, int *zeros) {

    char* codigo = calloc(sizeof(int)*8, sizeof(char));
    converterDeciBin(AC, codigo, (sizeof(int)*8));

    char* prefixo = calloc( (sizeof(int)*8) - categoria, sizeof(char) );
    char* _sufixo = calloc( categoria, sizeof(char) );

    strncpy(prefixo, codigo, (sizeof(int)*8) - categoria);
    strncpy(_sufixo, &codigo[strlen(codigo) - categoria], categoria);

    char *_prefixo;
    int length;

    for(int i=0; i<sizeof(int)*8; i++) if(prefixo[i] == '1' || i >= strlen(prefixo)-2) {

        length = (sizeof(int)*8) - i;

        _prefixo = calloc(length, sizeof(char));
        strcpy(_prefixo, &prefixo[i]);

        break;

    }

    int tmp;

    invertePrefixoAC(_prefixo, zeros, &tmp);
    inverteSufixoDCAC(_sufixo, categoria, valor);

    free(prefixo); 
    free(_prefixo);
    free(_sufixo); 
    free(codigo);

} 