// arquivo de cabeçalho com funções pra manipular arquivos
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

// lê arquivo e retorna seus bytes
U8* ler_arquivo(const char* caminho, size_t* tamanho);
// pega extensão tipo "txt" de "arquivo.txt"
char* pegar_extensao_arquivo(const char* caminho);
// muda extensão do arquivo tipo "arquivo.txt" pra "arquivo.doc"
char* setar_novo_nome(const char* nome_arquivo, const char* extensao_desejada);