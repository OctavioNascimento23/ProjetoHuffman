#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

// inicializa um novo código vazio
boolean novo_codigo (Codigo* c)
{
    c->byte = (U8*)calloc(1, sizeof(U8));
    if (c->byte == NULL) return FALSE;

    c->capacidade = 8;
    c->tamanho = 0;
    return TRUE;
}

// libera memória de um código
void free_codigo (Codigo* c)
{
    if (c->byte != NULL) free(c->byte);
    c->byte = NULL;
    c->capacidade = 0;
    c->tamanho = 0;
}

// adiciona um bit (0 ou 1) no fim do código
boolean adiciona_bit (Codigo* c, U8 valor)
{
    // aumenta buffer se necessário
    if (c->tamanho == c->capacidade)
    {
        U8* novo = (U8*)realloc(c->byte, (c->capacidade / 8 + 1) * sizeof(U8));
        if (novo == NULL) return FALSE;
        c->byte = novo;
        c->capacidade += 8;
    }

    // calcula posição do novo bit
    U8 byte_index = c->tamanho / 8;
    U8 bit_offset = c->tamanho % 8;

    // adiciona bit na posição correta
    if (valor == 1) {
        c->byte[byte_index] |= (1 << (7 - bit_offset));
    } else {
        c->byte[byte_index] &= ~(1 << (7 - bit_offset));
    }

    c->tamanho++;
    return TRUE;
}

// remove último bit do código
boolean joga_fora_bit (Codigo* c)
{
    if (c->tamanho == 0) return FALSE;

    U8 byte_index = (c->tamanho - 1) / 8;
    U8 bit_offset = (c->tamanho - 1) % 8;

    c->byte[byte_index] &= ~(1 << (7 - bit_offset));
    c->tamanho--;

    // reduz buffer se possível
    if (c->capacidade > 8 && (c->capacidade - c->tamanho) >= 8)
    {
        U8* novo = (U8*)realloc(c->byte, (c->capacidade / 8 - 1) * sizeof(U8));
        if (novo != NULL) {
            c->byte = novo;
            c->capacidade -= 8;
        }
    }
    return TRUE;
}

// cria cópia de um código
boolean clone (Codigo original, Codigo* copia)
{
    copia->byte = (U8*)malloc((original.capacidade + 7) / 8 * sizeof(U8));
    if (copia->byte == NULL) return FALSE;

    memcpy(copia->byte, original.byte, (original.capacidade + 7) / 8 * sizeof(U8));

    copia->capacidade = original.capacidade;
    copia->tamanho = original.tamanho;
    return TRUE;
}