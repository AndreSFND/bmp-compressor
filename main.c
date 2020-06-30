/**
 * O programa pode comprimir um arquivo bitmap e descomprimir um binario, retornando a um estado semelhante ao original
 * 
 * @author Andre Santana Fernandes <11208537>
*/

#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "compressor.h"

void finalizarPrograma(char* mensagem);

int main() {

    // Declara a variavel de operacao
    int operacao;

    // Declara as estruturas que armazenam os dados do arquivo bitmap
    BITMAP_FILEHEADER *fileHeader;
    BITMAP_INFOHEADER *infoHeader;
    BITMAP_RGB *RGB;

    // Declara as estruturas utilizadas na compressao 
    COMPRESSOR_YCBCR *YCbCr;
    COMPRESSOR_YCBCR *frequencias;

    // Inicializa os cabecalhos
    iniciaHeader(&fileHeader);
    iniciaInfoHeader(&infoHeader);

    // Enquanto a operacao for diferente de 2, continua operando
    while(operacao != 3) {

        // Imprime o menu
        printf("Escolha uma operação:\n");
        printf("1 - Compressão de uma imagem .bmp\n");
        printf("2 - Descompressão de uma imagem\n");
        printf("3 - Sair\n");

        // Le a operacao
        scanf("%d", &operacao);

        // Limpa a tela
        system("@cls||clear");

        // Executa a operacao desejada
        switch(operacao) {

            case 1: {

                // Declara e le o nome da imagem a ser comprimida
                /**
                 * barbara_gray.bmp 
                 * lena_gray.bmp
                 * BLK.bmp
                 * flag.bmp
                 * land.bmp
                 * land2.bmp
                 * land3.bmp
                 * marbles.bmp
                 * ray.bmp
                 * tru256.bmp
                 * venus.bmp
                 * WHT.bmp
                 * xing.bmp
                 * 
                */
                char* nomeArquivo = "./img/ray.bmp";
                //printf("Insira o nome da imagem a ser comprimida:\n");
                //scanf("%ms", &nomeArquivo);

                // Abertura dos arquivos de leitura e escrita
                FILE *arquivo;
                if((arquivo = fopen(nomeArquivo, "rb")) == NULL)
                    finalizarPrograma("Arquivo não encontrado");

                // Realiza a leitura das informacoes do cabecalho
                leituraHeader(arquivo, fileHeader);
                leituraInfoHeader(arquivo, infoHeader);

                // Le a altura e largura da imagem
                int Width = bitmapWidth(infoHeader);
                int Height = bitmapHeight(infoHeader);

                // Verifica se as dimensoes estao dentro do esperado
                if(Width < 8 || Height < 8 || Width > 1280 || Height > 800)
                    finalizarPrograma("Dimensões fora do esperado. Utilize uma imagem entre 8x8 e 1280x800 pixeis.");

                int bitcount = getBitCount(infoHeader);
                
                if(bitcount != 24) {
                
                    printf("Formato inválido (%d bits por pixel), escolha uma imagem com 24 bits por pixel.\n", bitcount);
                    return 0;
                
                }

                // Inicializa a matriz RGB e le as cores da imagem
                iniciaRGB(&RGB, infoHeader);
                leituraRGB(arquivo, RGB, infoHeader);

                // Inicializa a estrutura YCbCr e converte de RGB para YCbCr
                iniciaYCBCR(&YCbCr, infoHeader);
                RGBparaYCBCR(RGB, YCbCr, infoHeader);

                // Realiza o processo de downsampling
                DownSampling(&YCbCr, infoHeader);

                // ImprimeYCBCR(YCbCr, infoHeader);

                // Inicializa a matriz de frequencias
                // iniciaYCBCR(&frequencias, infoHeader);

                
                    // Cria uma nova imagem
                    FILE *arquivoGerado;
                    if((arquivoGerado = fopen("gerado.bmp", "w+b")) == NULL)
                        return 0;

                    // Escreve os dados de cabecalho
                    escreveHeader(arquivoGerado, fileHeader);
                    escreveInfoHeader(arquivoGerado, infoHeader);

                    // Converte de YCbCr para RGB
                    YCBCRparaRGB(RGB, YCbCr, infoHeader);

                    // Escreve a matriz de cores no formato RGB
                    escreveRGB(arquivoGerado, RGB, infoHeader);
                

                // Fecha os arquivos
                fclose(arquivoGerado);
                fclose(arquivo);

                // Libera memoria
                liberaRGB(&RGB, infoHeader);
                liberaYCBCR(&YCbCr, infoHeader);

                break;

            }

            case 2: { 

                // Declara e le o nome da imagem a ser descomprimida
                char* nomeArquivo;
                printf("Insira o nome da imagem a ser descomprimida:\n");
                scanf("%ms", &nomeArquivo);

                FILE *fptr;
                
                printf("Descompressao da imagem %s\n", nomeArquivo);

                break;

            }
            
            case 3: {

                exit(0); 

                break;

            }

            default: { 

                printf("Operação inválida");

                break;

            }

        }

        getchar();

    }

    // Libera memoria
    liberaHeader(&fileHeader);
    liberaInfoHeader(&infoHeader);

    return 0;

}

/**
 * Finaliza o programa, exibindo a mensagem desejada
 * 
 * @param char* mensagem mensagem a ser exibida no console
*/
void finalizarPrograma(char* mensagem) {

    printf("%s\n", mensagem);
    exit(0);

}