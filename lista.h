/**
 * Abstracao do TAD de uma lista
 * 
 * @author Andre Santana Fernandes <nUSP 11208537>
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

    bool Lista_Inserir(LISTA *Lista, int zeros, int valor);

    NO *Lista_No(LISTA *Lista, int index);
    NO *Lista_No_Proximo(NO *No);
    int Lista_No_Vertice(NO *No);

#endif