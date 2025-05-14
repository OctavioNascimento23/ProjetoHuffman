#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "types.h"
#include "arquivos.h"

int main() {
    // variáveis principais do programa
    int opcao;
    char nome_arquivo[256];
    char nome_saida[256];
    U8* dados_original = NULL;
    Tabela_de_frequencias* tabela = NULL;
    lista_t* lista = NULL;
    no_arvore* arvore = NULL;
    Codigo** dicionario = NULL;
    Codigo codigo_atual = {0};

    // menu principal
    printf("Selecione uma opção:\n");
    printf("1 - Codificar o arquivo\n");
    printf("2 - Decodificar o arquivo\n");
    printf("Opção: ");
    scanf("%d", &opcao);
    getchar();

    if (opcao == 1) {
        // modo compressão
        printf("Digite o nome do arquivo a ser codificado: ");
        fgets(nome_arquivo, sizeof(nome_arquivo), stdin);
        nome_arquivo[strcspn(nome_arquivo, "\n")] = 0;

        // lê arquivo original
        size_t tamanho_original;
        dados_original = ler_arquivo(nome_arquivo, &tamanho_original);
        if (!dados_original || tamanho_original == 0) {
            fprintf(stderr, "Erro: Falha ao ler arquivo original '%s' ou arquivo vazio.\n", nome_arquivo);
            limpeza_completa(dados_original, tabela, lista, arvore, dicionario, NULL);
            return EXIT_FAILURE;
        }
        printf("Arquivo original '%s' lido com sucesso. Tamanho: %zu bytes\n", nome_arquivo, tamanho_original);

        // cria e preenche tabela de frequências
        tabela = nova_tabela_de_frequencias();
        if (!tabela) {
            limpeza_completa(dados_original, tabela, lista, arvore, dicionario, NULL);
            return EXIT_FAILURE;
        }
        for (size_t i = 0; i < tamanho_original; i++) {
            if (!inclua_byte(dados_original[i], tabela)) {
                fprintf(stderr, "Erro: Falha ao incluir byte 0x%02X na tabela\n", dados_original[i]);
                limpeza_completa(dados_original, tabela, lista, arvore, dicionario, NULL);
                return EXIT_FAILURE;
            }
        }

        // cria lista ordenada por frequência
        printf("\n=== Lista Encadeada ===\n");
        lista = criar_lista_encadeada(tabela);
        if (!lista) {
            limpeza_completa(dados_original, tabela, lista, arvore, dicionario, NULL);
            return EXIT_FAILURE;
        }

        // monta árvore de huffman
        printf("\n=== Árvore de Huffman ===\n");
        arvore = criar_arvore(lista);
        if (!arvore) {
            limpeza_completa(dados_original, tabela, lista, arvore, dicionario, NULL);
            return EXIT_FAILURE;
        }

        // gera dicionário de códigos
        dicionario = alocar_dicionario();
        if (!dicionario) {
            limpeza_completa(dados_original, tabela, lista, arvore, dicionario, NULL);
            return EXIT_FAILURE;
        }
        if (!novo_codigo(&codigo_atual)) {
            limpeza_completa(dados_original, tabela, lista, arvore, dicionario, NULL);
            return EXIT_FAILURE;
        }
        gerar_dicionario(dicionario, arvore, codigo_atual);

        // comprime arquivo
        char nome_arquivo_codificado[260];
        snprintf(nome_arquivo_codificado, sizeof(nome_arquivo_codificado), "%s.huf", nome_arquivo);
        printf("\n=== Codificação: '%s' => '%s' ===\n", nome_arquivo, nome_arquivo_codificado);
        FILE* in = fopen(nome_arquivo, "rb");
        FILE* out = fopen(nome_arquivo_codificado, "wb");
        if (!in || !out) {
            perror("Erro ao abrir arquivos para codificação");
            if (in) fclose(in);
            if (out) fclose(out);
            limpeza_completa(dados_original, tabela, lista, arvore, dicionario, NULL);
            return EXIT_FAILURE;
        }
        codificar(in, out, dicionario);
        fclose(in);
        fclose(out);
        printf("Codificação concluída com sucesso: %s\n", nome_arquivo_codificado);
        limpeza_completa(dados_original, tabela, lista, arvore, dicionario, &codigo_atual);

    } else if (opcao == 2) {
        // modo descompressão
        printf("Digite o nome do arquivo .huf a ser decodificado: ");
        fgets(nome_arquivo, sizeof(nome_arquivo), stdin);
        nome_arquivo[strcspn(nome_arquivo, "\n")] = 0;
        printf("Digite o nome do arquivo de saída: ");
        fgets(nome_saida, sizeof(nome_saida), stdin);
        nome_saida[strcspn(nome_saida, "\n")] = 0;
        printf("\n=== Decodificação: '%s' => '%s' ===\n", nome_arquivo, nome_saida);

        // descomprime arquivo
        FILE* in = fopen(nome_arquivo, "rb");
        FILE* out = fopen(nome_saida, "wb");
        if (!in || !out) {
            perror("Erro ao abrir arquivos para decodificação");
            if (in) fclose(in);
            if (out) fclose(out);
            return EXIT_FAILURE;
        }
        decodificar(in, out);
        fclose(in);
        fclose(out);
        printf("Decodificação concluída com sucesso: %s\n", nome_saida);
    } else {
        printf("Opção inválida!\n");
    }
    return 0;
}

// limpa toda memória alocada pelo programa
void limpeza_completa(U8* dados, Tabela_de_frequencias* tab, lista_t* lista, no_arvore* arvore, Codigo** dicionario, Codigo* codigo) {
    printf("[DEBUG] Iniciando limpeza...\n");

    if (codigo) {
        free_codigo(codigo);
    }
    if (dicionario) {
        liberar_dicionario(dicionario);
    }
    if (arvore) {
        liberar_arvore(arvore);
    }
    if (lista) {
        free(lista);
    }
    if (tab) {
        free(tab);
    }
    if (dados) {
        free(dados);
    }

    printf("[DEBUG] Limpeza completa.\n");
}

//gcc arvore.c main.c codificador.c codigo.c decodificador.c dicionario.c arquivos.c lista_encad.c tabela_freq.c -o huffman