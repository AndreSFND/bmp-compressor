### Como funciona?

O programa é capaz de comprimir um bitmap e descomprimir um arquivo binário gerado por ele.  
Para a compressão ele realiza as seguintes etapas:
- Lê o arquivo;
- Converte as cores de RGB para YCrCb;
- Realiza um downsampling;
- Aplica a transformada DCT;
- Realiza a quantização;
- Realiza a codificação por entropia;
