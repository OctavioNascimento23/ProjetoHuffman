#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "types.h"

// cria arvore de huffman a partir de uma lista ordenada por frequencia
no_arvore* criar_arvore(lista_t* lista) {
    if(!lista) {
        printf("Parametro lista invalido.");
        return NULL;
    }

    no_arvore *esq, *dir;
    // junta os dois nos de menor frequencia ate sobrar so a raiz
    while(lista->tamanho > 1) {
        esq = remover_primeiro_no_lista(&lista);
        dir = remover_primeiro_no_lista(&lista);

        elemento_t* informacoes_nodo_pai = (elemento_t*)malloc(sizeof(elemento_t));
        if(informacoes_nodo_pai == NULL) {
            printf("Erro de alocacao de memoria (criar arvore) \n");
            return NULL;
        }

        // cria no pai com frequencia = soma dos filhos
        informacoes_nodo_pai->byte = 0; 
        informacoes_nodo_pai->frequencia = (esq->informacao.frequencia + dir->informacao.frequencia);

        no_arvore* nodo_pai_arvore =  criar_no(informacoes_nodo_pai);
        nodo_pai_arvore->esquerda = esq;
        nodo_pai_arvore->direita = dir;
        nodo_pai_arvore->proximo = NULL;

        inserir_ordenado(&lista, nodo_pai_arvore);
    }

    return lista->comeco;
}

// checa se um no é folha (nao tem filhos)
I8 eh_folha(no_arvore* raiz) {
    return(raiz->esquerda==NULL && raiz->direita==NULL);
}

// verifica se a arvore esta correta, mostrando os bytes nas folhas
void verificar_arvore(no_arvore* raiz) {
    if (!raiz) return;
    
    if (eh_folha(raiz)) {
        printf("Folha: %c (0x%02X)\n", 
               isprint(raiz->informacao.byte) ? raiz->informacao.byte : '.',
               raiz->informacao.byte);
    } else {
        if (!raiz->esquerda || !raiz->direita) {
            printf("ERRO: Nó interno com apenas um filho!\n");
        }
        verificar_arvore(raiz->esquerda);
        verificar_arvore(raiz->direita);
    }
}

// libera memoria da arvore inteira
void liberar_arvore(no_arvore* raiz) {
    if (!raiz) return;
    liberar_arvore(raiz->esquerda);
    liberar_arvore(raiz->direita);
    free(raiz);
}
