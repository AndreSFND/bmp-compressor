/**
 * TAD de uma lista
 * 
 * @author  Andre Santana Fernandes <11208537>
 * @author  Diogo Castanho Emidio <11297274>
 * @author  Leonardo Antonetti da Motta <11275338>
*/

#include "lista.h"
#include <stdio.h>

/**
 * Estrutura da lista;
 * Possui um no de inicio e um de fim, alem de um inteiro que armazena o tamanho da lista.
*/
struct lista_ {

    NO *inicio;
    NO *fim;

    int tamanho;

};

/**
 * Estrutura dos nos;
 * Armazena um inteiro com o vertice (valor) do no, alem do proximo no.
*/
struct no_ {

    int zeros;
    int valor;
    int categoria;

    NO *proximo;
    
};

/**
 * Cria uma lista
 * 
 * @returns LISTA* a lista criada
*/
LISTA *Lista_Criar() {

    //Aloca memoria dinamicamente
    LISTA *Lista;
    Lista = (LISTA*) malloc(sizeof(LISTA));

    if(Lista != NULL){

        Lista->inicio   = NULL;
        Lista->fim      = NULL;

        Lista->tamanho  = 0;

    }

    return Lista;

}

/**
 * Apaga uma lista
 * 
 * @param LISTA **ptr ponteiro da lista a ser apagada
*/
void Lista_Apagar(LISTA **ptr) {

    NO *no_atual;
    no_atual = (*ptr)->inicio;

    while(no_atual != NULL){

        NO *proximo = no_atual->proximo;

        free(no_atual);
        no_atual = proximo;

    }

    free(*ptr);

}

/**
 * Verifica se uma lista esta cheia
 * 
 * @param LISTA *Lista a ser analisada
 * 
 * @returns bool booleano que informa se a lista esta cheia
*/
bool Lista_Vazia(LISTA *Lista) {

    return (Lista->inicio == NULL);

}

/**
 * Verifica se uma lista esta vazia
 * 
 * @param LISTA *Lista a ser analisada
 * 
 * @returns bool booleano que informa se a lista esta vazia
*/
bool Lista_Cheia(LISTA *Lista) {

    NO *no;
    no = (NO *) malloc(sizeof(NO));

    return (no == NULL);

}


/**
 * Insere um vertice na lista
 * 
 * @param LISTA *Lista a ser manipulada
 * @param int vertice a ser inserido
 * 
 * @returns bool booleano que informa se o vertice foi inserido
*/
bool Lista_Inserir(LISTA *Lista, int zeros, int valor, int categoria) {

    if(!Lista_Cheia(Lista) && Lista != NULL){

        NO *novo_no = (NO *) malloc(sizeof(NO));

        if(Lista->tamanho == 0)    
            Lista->inicio       = novo_no;
        else
            Lista->fim->proximo = novo_no;
        
        novo_no->zeros = zeros;
        novo_no->valor = valor;
        novo_no->categoria = categoria;
        novo_no->proximo = NULL;

        Lista->fim = novo_no;
        Lista->tamanho++;

        return true;

    }

    return false;

}

/**
 * Retorna um no de uma lista
 * 
 * @param LISTA *Lista a ser manipulada
 * @param int index do no a ser retornado
 * 
 * @returns NO* no desejado
*/
NO *Lista_No(LISTA *Lista, int index) {

    if(Lista != NULL) {

        NO *no_atual;
        no_atual = Lista->inicio;
        
        int atual = 0;

        //Encontra o no desejado
        while(atual < index && no_atual != NULL) {

            no_atual = no_atual->proximo;
            atual++;

        }

        if(no_atual != NULL)
            return no_atual;

    }

    return NULL;

}

/**
 * Retorna o proximo no de um outro no
 * 
 * @param NO *No a ser manipulado
 * 
 * @returns NO* proximo no
*/
NO *Lista_No_Proximo(NO *No) {

    if(No != NULL)
        return No->proximo;

    return NULL;

}

int Lista_No_Zeros(NO *No) {

    if(No != NULL)
        return No->zeros;

    return -1;

}

int Lista_No_Valor(NO *No) {

    if(No != NULL)
        return No->valor;

    return -1;

}

int Lista_No_Categoria(NO *No) {

    if(No != NULL)
        return No->categoria;

    return -1;

}