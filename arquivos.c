// esse arquivo cuida de todas as operações com arquivos do programa
// como ler arquivos, pegar extensões e criar novos nomes de arquivo
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

// define o tamanho máximo de cada pedaço de arquivo que vamos ler por vez
// isso ajuda a não sobrecarregar a memória
#define CHUNK_SIZE 65536

// função que lê um arquivo inteiro e retorna seus bytes
// recebe o caminho do arquivo e um ponteiro pra guardar o tamanho
// retorna um ponteiro pros bytes do arquivo ou null se der erro
U8* ler_arquivo(const char* caminho, size_t* tamanho) {
    FILE* arquivo = fopen(caminho, "rb");
    if (!arquivo) {
        fprintf(stderr, "Erro ao abrir arquivo %s\n", caminho);
        return NULL;
    }

    // descobre o tamanho do arquivo
    fseek(arquivo, 0, SEEK_END);
    *tamanho = ftell(arquivo);
    rewind(arquivo);

    // checa se o arquivo não é grande demais (limite de 100mb)
    if (*tamanho > 100 * 1024 * 1024) {
        fprintf(stderr, "Arquivo muito grande. Use processamento por blocos.\n");
        fclose(arquivo);
        return NULL;
    }

    // aloca memória pro buffer que vai guardar o conteúdo
    U8* buffer = malloc(*tamanho + 1);
    if (!buffer) {
        fclose(arquivo);
        fprintf(stderr, "Falha na alocação de %zu bytes\n", *tamanho);
        return NULL;
    }

    // lê o arquivo em pedaços pra não sobrecarregar a memória
    size_t lidos = 0;
    while (lidos < *tamanho) {
        size_t chunk = (*tamanho - lidos) > CHUNK_SIZE ? 
                      CHUNK_SIZE : (*tamanho - lidos);
        size_t ret = fread(buffer + lidos, 1, chunk, arquivo);
        if (ret != chunk) {
            free(buffer);
            fclose(arquivo);
            fprintf(stderr, "Erro de leitura na posição %zu\n", lidos);
            return NULL;
        }
        lidos += ret;
    }
    fclose(arquivo);

    buffer[*tamanho] = '\0';
    return buffer;
}

// função que pega só a extensão de um arquivo
// exemplo: de "arquivo.txt" ela retorna "txt"
char* pegar_extensao_arquivo(const char* caminho) {
    I8 i = 0, j = 0;
    boolean encontrou = FALSE;    
    char* extensao = NULL;

    while(caminho[i] != '\0') {
        if(caminho[i] == '.') {
            encontrou = TRUE;
            free(extensao);
        } else if (encontrou) {
            char* temp = realloc(extensao, j+2);
            if(!temp) {
                free(extensao);
                return NULL;
            }

            extensao = temp;
            extensao[j++] = caminho[i];
            extensao[j] = '\0';
        }
        i++;
    } 

    return extensao;
}

// função que conta quantos caracteres tem numa string
I8 calcular_tamanho_string(const char* string) {
    I8 tamanho_string = 0;
    for(I8 i=0; string[i] != '\0'; i++) {
        tamanho_string++;
    }

    return tamanho_string;
}

// função que cria um novo nome de arquivo com uma extensão diferente
// exemplo: de "arquivo.txt" e "doc" ela faz "arquivo.doc"
char* setar_novo_nome(const char* nome_arquivo, const char* extensao_desejada) {
    I8 i = 0;
    while(nome_arquivo[i] != '\0' && nome_arquivo[i] != '.') {
        i++;
    }
    
    I8 tam_novo = i + (I8)1 + calcular_tamanho_string(extensao_desejada);
    char* novo_nome = malloc(tam_novo);
    if(!novo_nome) return NULL;
   
    I8 j=0;
    while(j!=i) {
        novo_nome[j] = nome_arquivo[j];
        j++;
    }

    novo_nome[j++]='.';
    strcpy(novo_nome + j, extensao_desejada);
    return novo_nome;
}

