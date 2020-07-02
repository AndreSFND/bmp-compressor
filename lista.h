/**
 * Abstracao do TAD de uma lista
 * 
 * @author  Andre Santana Fernandes <11208537>
 * @author  Diogo Castanho Emidio <11297274>
 * @author  Leonardo Antonetti da Motta <11275338>
*/

#ifndef LISTA_H
#define LISTA_H

    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>

    typedef struct lista_   LISTA;
    typedef struct no_      NO;

    LISTA *Lista_Criar();
    void Lista_Apagar(LISTA **ptr);
    bool Lista_Cheia(LISTA *Lista);
    bool Lista_Vazia(LISTA *Lista);

    bool Lista_Inserir(LISTA *Lista, int zeros, int valor, int categoria);

    NO *Lista_No(LISTA *Lista, int index);
    NO *Lista_No_Proximo(NO *No);

    int Lista_No_Zeros(NO *No);
    int Lista_No_Valor(NO *No);
    int Lista_No_Categoria(NO *No);

#endif