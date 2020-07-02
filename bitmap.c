/**
 * O TAD eh responsavel por manipular as informacoes do arquivo bitmap
 * 
 * @author Andre Santana Fernandes <11208537>
*/

#include "bitmap.h"

/**
 * Estrutura responsavel por armazenar os dados do cabecalho da imagem
 * Signature deve ser = "BM"
*/
struct bitmapfileheader {        

    unsigned short Signature; /* Numero magico */
    unsigned int FileSize; /* Tamanho do arquivo */        
    unsigned int Reserved; /* Reservado */        
    unsigned int DataOffset; /* Offset para os dados do bitmap */

};

/**
 * Estrutura responsavel por armazenar os dados do cabecalho de informacoes da imagem
*/
struct bitmapinfoheader {       

    unsigned int Size; /* Tamanho do cabecalho de informacoes */       
    int Width; /* Largura da imagem */       
    int Height; /* Altura da imagem */       
    unsigned short Planes; /* Numero de cores planas */       
    unsigned short BitCount; /* Numero de bits por pixel */       
    unsigned int Compression; /* Tipo de compressao a ser utilizada */       
    unsigned int ImageSize; /* Tamanho da imagem */
    int XPixelsPerMeter; /* X pixeis por metro */       
    int YPixelsPerMeter; /* Y pixeis por metro */       
    unsigned int ColorsUsed; /* Numero de cores utilizadas */       
    unsigned int ColorsImportant; /* Numero de cores importantes */

};

/**
 * Estrutura responsavel por armazenar as cores da imagem no formato RGB
*/
struct bitmaprgb {       

    unsigned char **R;
    unsigned char **G;
    unsigned char **B;

};

/**
 * Inicializa o cabecalho
 * 
 * @param BITMAP_FILEHEADER** fileHeader endereco da estrutura do cabecalho
*/
void iniciaHeader(BITMAP_FILEHEADER** fileHeader) {

    *fileHeader = malloc(sizeof(BITMAP_FILEHEADER));

}

/**
 * Libera o cabecalho
 * 
 * @param BITMAP_FILEHEADER** fileHeader endereco da estrutura do cabecalho
*/
void liberaHeader(BITMAP_FILEHEADER** fileHeader) {

    free(*fileHeader);

}

/**
 * Le e armazena os dados do cabecalho
 * 
 * @param FILE *file ponteiro para o arquivo
 * @param BITMAP_FILEHEADER *fileHeader estrutura do cabecalho
*/
void leituraHeader(FILE *file, BITMAP_FILEHEADER *fileHeader) {

    fread(&fileHeader->Signature, sizeof (unsigned short int), 1, file);
    fread(&fileHeader->FileSize, sizeof (unsigned int), 1, file);
    fread(&fileHeader->Reserved, sizeof (unsigned int), 1, file);
    fread(&fileHeader->DataOffset, sizeof (unsigned int), 1, file);
    
}

/**
 * Escreve os dados do cabecalho no arquivo
 * 
 * @param FILE *file ponteiro para o arquivo
 * @param BITMAP_FILEHEADER *fileHeader estrutura do cabecalho
*/
void escreveHeader(FILE *file, BITMAP_FILEHEADER *fileHeader) {

    fwrite(&fileHeader->Signature, sizeof (unsigned short int), 1, file);
    fwrite(&fileHeader->FileSize, sizeof (unsigned int), 1, file);
    fwrite(&fileHeader->Reserved, sizeof (unsigned int), 1, file);
    fwrite(&fileHeader->DataOffset, sizeof (unsigned int), 1, file);

}

/**
 * Inicializa o cabecalho de informacoes
 * 
 * @param BITMAP_INFOHEADER** infoHeader endereco da estrutura do cabecalho de informacoes
*/
void iniciaInfoHeader(BITMAP_INFOHEADER** infoHeader) {

    *infoHeader = malloc(sizeof(BITMAP_INFOHEADER));

}

/**
 * Libera o cabecalho de informacoes
 * 
 * @param BITMAP_INFOHEADER** infoHeader endereco da estrutura do cabecalho de informacoes
*/
void liberaInfoHeader(BITMAP_INFOHEADER** infoHeader) {

    free(*infoHeader);

}

/**
 * Le e armazena os dados do cabecalho de informacoes
 * 
 * @param FILE *file ponteiro para o arquivo
 * @param BITMAP_FILEHEADER *fileHeader estrutura do cabecalho de informacoes
*/
void leituraInfoHeader(FILE *file, BITMAP_INFOHEADER *infoHeader) {

    fread(&infoHeader->Size, sizeof (unsigned int), 1, file);
    fread(&infoHeader->Width, sizeof (int), 1, file);
    fread(&infoHeader->Height, sizeof (int), 1, file);
    fread(&infoHeader->Planes, sizeof (unsigned short int), 1, file);
    fread(&infoHeader->BitCount, sizeof (unsigned short int), 1, file);
    fread(&infoHeader->Compression, sizeof (unsigned int), 1, file);
    fread(&infoHeader->ImageSize, sizeof (unsigned int), 1, file);
    fread(&infoHeader->XPixelsPerMeter, sizeof (int), 1, file);
    fread(&infoHeader->YPixelsPerMeter, sizeof (int), 1, file);
    fread(&infoHeader->ColorsUsed, sizeof (unsigned int), 1, file);
    fread(&infoHeader->ColorsImportant, sizeof (unsigned int), 1, file);

    if(infoHeader->Width < 0) infoHeader->Width *= -1;
    if(infoHeader->Height < 0) infoHeader->Height *= -1;

}

/**
 * Escreve os dados do cabecalho de informacoes no arquivo
 * 
 * @param FILE *file ponteiro para o arquivo
 * @param BITMAP_FILEHEADER *fileHeader estrutura do cabecalho de informacoes
*/
void escreveInfoHeader(FILE *file, BITMAP_INFOHEADER *infoHeader) {

    fwrite(&infoHeader->Size, sizeof (unsigned int), 1, file);
    fwrite(&infoHeader->Width, sizeof (int), 1, file);
    fwrite(&infoHeader->Height, sizeof (int), 1, file);
    fwrite(&infoHeader->Planes, sizeof (unsigned short int), 1, file);
    fwrite(&infoHeader->BitCount, sizeof (unsigned short int), 1, file);
    fwrite(&infoHeader->Compression, sizeof (unsigned int), 1, file);
    fwrite(&infoHeader->ImageSize, sizeof (unsigned int), 1, file);
    fwrite(&infoHeader->XPixelsPerMeter, sizeof (int), 1, file);
    fwrite(&infoHeader->YPixelsPerMeter, sizeof (int), 1, file);
    fwrite(&infoHeader->ColorsUsed, sizeof (unsigned int), 1, file);
    fwrite(&infoHeader->ColorsImportant, sizeof (unsigned int), 1, file);

}

/**
 * Inicializa uma estrutura de cores no formato RGB 8x8
 * 
 * @param BITMAP_RGB** RGB endereco da estrutura de cores no formato RGB
*/
void iniciaRGB(BITMAP_RGB** RGB) {

    *RGB = malloc(sizeof(BITMAP_RGB));

    (*RGB)->R = malloc( sizeof(unsigned char*) * HEIGHT );
    for(int i=0; i<HEIGHT; i++)
        (*RGB)->R[i] = malloc( sizeof(unsigned char) * WIDTH );

    (*RGB)->G = malloc( sizeof(unsigned char*) * HEIGHT );
    for(int i=0; i<HEIGHT; i++)
        (*RGB)->G[i] = malloc( sizeof(unsigned char) * WIDTH );

    (*RGB)->B = malloc( sizeof(unsigned char*)* HEIGHT );
    for(int i=0; i<HEIGHT; i++)
        (*RGB)->B[i] = malloc( sizeof(unsigned char) * WIDTH );

}

/**
 * Libera uma estrutura de cores no formato RGB 8x8
 * 
 * @param BITMAP_RGB** RGB endereco da estrutura de cores no formato RGB
*/
void liberaRGB(BITMAP_RGB** RGB) {

    for(int i=0; i<HEIGHT; i++)
        free((*RGB)->R[i]);
    free((*RGB)->R);

    for(int i=0; i<HEIGHT; i++)
        free((*RGB)->G[i]);
    free((*RGB)->G);

    for(int i=0; i<HEIGHT; i++)
        free((*RGB)->B[i]);
    free((*RGB)->B);

    free(*RGB);

}

/**
 * Le e armazena as cores de um bloco 8x8 da imagem no formato RGB
 * 
 * @param FILE *file ponteiro para o arquivo
 * @param BITMAP_RGB* RGB estrutura de cores no formato RGB
*/
void leituraRGB(FILE *file, BITMAP_RGB *RGB) {

    for(int i=0; i<HEIGHT; i++) {

        for(int j=0; j<WIDTH; j++) {

            fread( &( RGB->B[i][j] ), sizeof(unsigned char), 1, file);
            fread( &( RGB->G[i][j] ), sizeof(unsigned char), 1, file);
            fread( &( RGB->R[i][j] ), sizeof(unsigned char), 1, file);

        }

    }

}

/**
 * Escreve as cores de um bloco 8x8 da imagem, no formato RGB, no arquivo
 * 
 * @param FILE *file ponteiro para o arquivo
 * @param BITMAP_RGB* RGB estrutura de cores no formato RGB
*/
void escreveRGB(FILE *file, BITMAP_RGB *RGB) {

    for(int i=0; i<HEIGHT; i++) {

        for(int j=0; j<WIDTH; j++) {

            fwrite( &( RGB->B[i][j] ), sizeof(unsigned char), 1, file);
            fwrite( &( RGB->G[i][j] ), sizeof(unsigned char), 1, file);
            fwrite( &( RGB->R[i][j] ), sizeof(unsigned char), 1, file);

        }

    }

}

/**
 * Retorna a largura da imagem
 * 
 * @param FILE *file ponteiro para o arquivo
 * @returns int largura da imagem
*/
int bitmapWidth(BITMAP_INFOHEADER *infoHeader) {

    return infoHeader->Width;

}

/**
 * Retorna a altura da imagem
 * 
 * @param FILE *file ponteiro para o arquivo
 * @returns int altura da imagem
*/
int bitmapHeight(BITMAP_INFOHEADER *infoHeader) {

    return infoHeader->Height;

}

/**
 * Retorna a intensidade da cor R em algum ponto da imagem
 * 
 * @param BITMAP_RGB* RGB estrutura de cores no formato RGB
 * @param int i coluna
 * @param int j linha
*/
unsigned char getBitmapR(BITMAP_RGB *RGB, int i, int j) {

    return RGB->R[i][j];

}

/**
 * Retorna a intensidade da cor G em algum ponto da imagem
 * 
 * @param BITMAP_RGB* RGB estrutura de cores no formato RGB
 * @param int i coluna
 * @param int j linha
*/
unsigned char getBitmapG(BITMAP_RGB *RGB, int i, int j) {

    return RGB->G[i][j];

}

/**
 * Retorna a intensidade da cor B em algum ponto da imagem
 * 
 * @param BITMAP_RGB* RGB estrutura de cores no formato RGB
 * @param int i coluna
 * @param int j linha
*/
unsigned char getBitmapB(BITMAP_RGB *RGB, int i, int j) {

    return RGB->B[i][j];

}

/**
 * Define a intensidade da cor R em algum ponto da imagem
 * 
 * @param BITMAP_RGB* RGB estrutura de cores no formato RGB
 * @param int i coluna
 * @param int j linha
 * @param unsigned char value valor a ser definido
*/
void setBitmapR(BITMAP_RGB *RGB, int i, int j, unsigned char value) {

    RGB->R[i][j] = value;

}

/**
 * Define a intensidade da cor G em algum ponto da imagem
 * 
 * @param BITMAP_RGB* RGB estrutura de cores no formato RGB
 * @param int i coluna
 * @param int j linha
 * @param unsigned char value valor a ser definido
*/
void setBitmapG(BITMAP_RGB *RGB, int i, int j, unsigned char value) {

    RGB->G[i][j] = value;

}

/**
 * Define a intensidade da cor B em algum ponto da imagem
 * 
 * @param BITMAP_RGB* RGB estrutura de cores no formato RGB
 * @param int i coluna
 * @param int j linha
 * @param unsigned char value valor a ser definido
*/
void setBitmapB(BITMAP_RGB *RGB, int i, int j, unsigned char value) {

    RGB->B[i][j] = value;

}

/**
 * Retorna a quantidade de bits por pixel
 * 
 * @param BITMAP_INFOHEADER *infoHeader estrutura do cabecalho de informacoes
 * @returns int quantidade de bits por pixel
*/
int getBitCount(BITMAP_INFOHEADER *infoHeader) {

    return infoHeader->BitCount;

}