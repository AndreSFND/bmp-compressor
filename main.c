/**
 * USP Sao Carlos, ICMC
 * Projeto de Multimidia (SCC0561) - Compressao de Imagens
 * 
 * O programa pode comprimir um arquivo bitmap e descomprimir um binario, retornando a um estado semelhante ao original
 * 
 * @author  Andre Santana Fernandes <11208537>
 * @author  Diogo Castanho Emidio <11297274>
 * @author  Leonardo Antonetti da Motta <11275338>
*/

#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "compressor.h"
#include "lista.h"

void finalizarPrograma(char* mensagem);

int main() {

    // Declara a variavel de operacao
    int operacao;

    // Declara as estruturas que armazenam os dados do arquivo bitmap
    BITMAP_FILEHEADER *fileHeader;
    BITMAP_INFOHEADER *infoHeader;    

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
                char* nomeArquivo = "./img/goldhill.bmp";
                // printf("Insira o nome da imagem a ser comprimida:\n");
                // scanf("%ms", &nomeArquivo);

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

                // Cria o arquivo comprimido
                FILE *arquivoGerado;
                if((arquivoGerado = fopen("comprimido.bin", "w+b")) == NULL)
                    return 0;

                // Escreve os dados de cabecalho
                escreveHeader(arquivoGerado, fileHeader);
                escreveInfoHeader(arquivoGerado, infoHeader);
                
                // Inicializa os vetores de codificacao por diferenca de DC
                int ultimoDCY = 0;
                int ultimoDCCb = 0;
                int ultimoDCCr = 0;
                
                for(int i=0; i<Height/8; i++) {

                    for(int j=0; j<Width/8; j++) {

                        // Inicializa a matriz RGB e le as cores da imagem
                        BITMAP_RGB *RGB;
                        iniciaRGB(&RGB);
                        leituraRGB(arquivo, RGB);

                         // Inicializa a estrutura YCbCr e converte de RGB para YCbCr
                        COMPRESSOR_YCBCR *YCbCr;
                        iniciaYCBCR(&YCbCr);
                        RGBparaYCBCR(RGB, YCbCr);
                        liberaRGB(&RGB);

                        // Realiza o processo de downsampling
                        // TODO: o downsampling deve ficar fora do loop
                        //DownSampling(&YCbCr);

                        // Aplica a transformada DCT
                        COMPRESSOR_YCBCR *frequencias;
                        iniciaYCBCR(&frequencias);
                        transformadaDCT(frequencias, YCbCr);
                        liberaYCBCR(&YCbCr);

                        // Aplica a quantizacao
                        COMPRESSOR_YCBCR *quantizada;
                        iniciaYCBCR(&quantizada);
                        Quantizacao(quantizada, frequencias);
                        liberaYCBCR(&frequencias);

                        // Vetoriza a matriz quantizada
                        COMPRESSOR_MATRIZ *vetorizados;
                        iniciaMatriz(&vetorizados, WIDTH*HEIGHT);
                        Vetorizacao(quantizada, vetorizados);
                        liberaYCBCR(&quantizada);

                        // Codificacao por diferenca dos coeficientes DC
                        int index = i*Height + j;
                        int DCY = getMatrizValue(vetorizados, 0, 0) - ultimoDCY;
                        int DCCb = getMatrizValue(vetorizados, 1, 0) - ultimoDCCb;
                        int DCCr = getMatrizValue(vetorizados, 2, 0) - ultimoDCCr;

                        ultimoDCY = getMatrizValue(vetorizados, 0, 0);
                        ultimoDCCb = getMatrizValue(vetorizados, 1, 0);
                        ultimoDCCr = getMatrizValue(vetorizados, 2, 0);

                        // Codificacao run length dos coeficientes AC
                        COMPRESSOR_LISTAS *codificadosAC;
                        iniciaListas(&codificadosAC);
                        RunLength(vetorizados, codificadosAC);
                        liberaMatriz(&vetorizados, WIDTH*HEIGHT);

                        // Codificacao estatistica e escrita dos dados no arquivo binario
                        codificacaoEstatistica(arquivoGerado, DCY, DCCb, DCCr, codificadosAC); 
                        liberaListas(&codificadosAC);     

                    }

                }
               
                // Fecha os arquivos
                fclose(arquivoGerado);
                fclose(arquivo);

                break;

            }

            case 2: { 

                // Declara e le o nome da imagem a ser descomprimida
                char* nomeArquivo = "comprimido.bin";
                // printf("Insira o nome da imagem a ser descomprimida:\n");
                // scanf("%ms", &nomeArquivo);

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

                // Cria uma nova imagem
                FILE *arquivoGerado;
                if((arquivoGerado = fopen("gerado.bmp", "w+b")) == NULL)
                    return 0;

                // Escreve os dados de cabecalho
                escreveHeader(arquivoGerado, fileHeader);
                escreveInfoHeader(arquivoGerado, infoHeader);

                // Inicializa os vetores de codificacao por diferenca de DC
                int ultimoDCY = 0;
                int ultimoDCCb = 0;
                int ultimoDCCr = 0;

                for(int i=0; i<Height/8; i++) {

                    for(int j=0; j<Width/8; j++) {

                        int DCY, DCCb, DCCr;

                        COMPRESSOR_LISTAS *codificadosAC;
                        iniciaListas(&codificadosAC);
                        leituraEstatistica(arquivo, &DCY, &DCCb, &DCCr, codificadosAC);

                        // Inverte o processo de run length
                        COMPRESSOR_MATRIZ *vetorizados;
                        iniciaMatriz(&vetorizados, WIDTH*HEIGHT);
                        inversaRunLength(vetorizados, codificadosAC);
                        liberaListas(&codificadosAC);

                        // Define os DCs
                        DCY += ultimoDCY;
                        DCCb += ultimoDCCb;
                        DCCr += ultimoDCCr;

                        ultimoDCY = DCY;
                        ultimoDCCb = DCCb;
                        ultimoDCCr = DCCr;

                        setMatrizValue(vetorizados, 0, 0, DCY);
                        setMatrizValue(vetorizados, 1, 0, DCCb);
                        setMatrizValue(vetorizados, 2, 0, DCCr);

                        // Inverte a vetorizacao
                        COMPRESSOR_YCBCR *quantizada;
                        iniciaYCBCR(&quantizada);
                        inversaVetorizacao(quantizada, vetorizados);
                        liberaMatriz(&vetorizados, WIDTH*HEIGHT);

                        // Aplica a inversa da quantizada
                        COMPRESSOR_YCBCR *frequencias;
                        iniciaYCBCR(&frequencias);
                        inversaQuantizada(quantizada, frequencias);
                        liberaYCBCR(&quantizada);

                        // Aplica a inversa da transformada DCT
                        COMPRESSOR_YCBCR *YCbCr;
                        iniciaYCBCR(&YCbCr);
                        inversaDCT(frequencias, YCbCr);
                        iniciaYCBCR(&frequencias);

                        // Converte de YCbCr para RGB e Escreve a matriz de cores no formato RGB
                        BITMAP_RGB *RGB;
                        iniciaRGB(&RGB);
                        YCBCRparaRGB(RGB, YCbCr);
                        liberaYCBCR(&YCbCr);
                        
                        // Escreve os dados da data unit
                        escreveRGB(arquivoGerado, RGB);
                        liberaRGB(&RGB);                

                    }

                }

                // Fecha os arquivos
                fclose(arquivoGerado);
                fclose(arquivo);

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