Projeto de Multimídia (SCC0561) - Compressão de Imagens
Nomes:	Andre Santana Fernandes - 11208537
	Diogo Castanho Emidio - 11297274
	Leonardo Antonetti da Motta - 11275338

Instruções de uso:
	1. Colocar o programa dentro de uma pasta com imagens para serem comprimidas;
	2. Usar o Makefile para compilar o programa;
	3. Iniciar o programa;
	4. Escolher uma das opções (1 - comprimir, 2 - descomprimir, 3 - sair);
	5. Digitar o nome da imagem BMP (imagem.bmp) a ser comprida (opção 1) ou do arquivo binário (arquivo.bin) a ser descomprimido (opção 2);
	6. No caso da opção 1, escolher um dos fatores de compressão/quantização

Especificações das imagens de entrada:
	Comprimento e largura múltiplos de 8;
	24 bits por píxel

Saídas do programa:
	Caso 1:
		Arquivo binário (comprimido);
		Tamanhos da imagem e do arquivo;
		Proporção entre a imagem e o arquivo
	Caso 2:
		Imagem bmp (descomprimido)

Organização:
	TADs:
		bitmap: organiza as informações do bitmap;
		compressor: realiza a compressão e a descompressão propriamente ditas;
		lista: utilizada na compressão por carreira
	Compressão:
		1. Preparação de imagem/bloco
			1.1. Lê o arquivo BMP e separa o header;
			1.2. Verifica a compatibilidade do arquivo;
			1.3. Separa a matriz de píxeis em matrizes R, G e B;
			1.4. Converte as matrizes RGB em YCbCr;
			1.5. Separa as matrizes em data units de 8x8 píxeis;
			1.6. Utiliza-se o algoritmo 4:2:2 para subamostragem de crominância;
		2. Aplica a DCT;
		3. Realiza a quantização, usando as matrizes do Microsoft Paint;
		4. Codificação por entropia
			4.1. Codifica os coeficientes DC por diferença;
			4.2. Vetoriza os coeficientes;
			4.3. Cofifica os coeficientes AC por carreira;
			4.4. Codifica os coeficientes por estatística, utilizando o método e as tabelas do padrão JPEG;
			4.5. Escreve como arquivo binário ("frame builder")
	Descompressão:
		1. Decodificação do arquivo
			1.1. Lê o arquivo binário e separa o header;
			1.2. Verifica a compatibilidade do arquivo
		2. Decodificação por entropia
			2.1. Decodifica os coeficientes por estatística, utilizando o método e as tabelas do padrão JPEG;
			2.2. Decofifica os coeficientes AC por carreira;
			2.3. Decodifica os coeficientes DC por diferença
		3. Realiza a desquantização, usando as matrizes do Microsoft Paint;
		4. Aplica a IDCT;
		5. Escreve como arquivo BMP ("image builder")
			5.1. Utiliza-se o algoritmo 4:2:2 para subamostragem de crominância;
			5.2. Agrupa data units de 8x8 píxeis em matrizes;
			5.3. Converte as matrizes de YCbCr para RGB;
			5.4. Agrupa as matrizes R, G e B em uma matriz de píxeis

Separação:
	Andre Santana Fernandes (11208537):
		Codificação por entropia;
		Decodificação por entropia
	Diogo Castanho Emidio (11297274):
		Realiza a quantização, usando as matrizes do Microsoft Paint;
		Realiza a desquantização, usando as matrizes do Microsoft Paint;
		Escreve como arquivo BMP ("image builder")
	Leonardo Antonetti da Motta (11275338):
		Preparação de imagem/bloco;
		Aplica a DCT;
		Decodificação do arquivo;
		Aplica a IDCT