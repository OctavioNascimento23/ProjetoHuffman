#include <stdio.h>
#include <stdlib.h>
#include "types.h"

// cria um nó da árvore com as informações do byte e frequência
no_arvore* criar_no(elemento_t* elemento) {
    no_arvore* nodo = (no_arvore*)malloc(sizeof(no_arvore));
    if(nodo == NULL) {
        printf("Ocorreu um erro na alocacao de memoria. (criar_no)\n");
        return NULL; 
    }

    // inicializa nó vazio
    nodo->direita=NULL;
    nodo->esquerda=NULL;
    nodo->proximo=NULL;
    nodo->informacao = *(elemento);

    return nodo;
}

// insere nó na lista mantendo ordem crescente de frequência
void inserir_ordenado(lista_t** lista, no_arvore* nodo) {
    no_arvore* novo_nodo = nodo;
    no_arvore* atual = (*lista)->comeco;
    no_arvore* anterior = NULL;

    // lista vazia: insere no começo
    if (atual == NULL) {
        (*lista)->comeco = novo_nodo;
        novo_nodo->proximo = NULL;
        (*lista)->tamanho++;
        return;
    }

    // procura posição certa pela frequência
    while (atual != NULL && novo_nodo->informacao.frequencia > atual->informacao.frequencia) {
        anterior = atual;
        atual = atual->proximo;
    }

    // insere no início
    if (anterior == NULL) {
        novo_nodo->proximo = (*lista)->comeco;
        (*lista)->comeco = novo_nodo;
    }
    // insere no meio/fim
    else {
        novo_nodo->proximo = atual;
        anterior->proximo = novo_nodo;
    }

    (*lista)->tamanho++;
}

// cria lista com nós da tabela de frequência
lista_t* criar_lista_encadeada(Tabela_de_frequencias* tabela) {
    lista_t* lista = (lista_t*)malloc(sizeof(lista_t));
    if(lista == NULL) {
        printf("Erro ao alocar memoria para lista. (criar_lista_encadeada).\n");
        return NULL;
    }

    // inicializa lista vazia
    lista->comeco=NULL;
    lista->tamanho=0;
    
    // insere cada byte que aparece no arquivo
    for(U16 i=0; i< 256; i++) {
        if(tabela->vetor[i] != NULL) {
            inserir_ordenado(&lista, tabela->vetor[i]);
        }
    }

    return lista;
}

// remove e retorna primeiro nó da lista
no_arvore* remover_primeiro_no_lista(lista_t** lista) {
    if((*lista)->tamanho == 0) {
        printf("Lista nao possui elementos para serem removidos.");
        return NULL;
    }

    no_arvore* aux = (*lista)->comeco;
    (*lista)->comeco = aux->proximo;
    aux->proximo = NULL; 
    (*lista)->tamanho--;

    return aux;
}

