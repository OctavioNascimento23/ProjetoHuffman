# Codificação Huffman

## Descrição

Este projeto implementa o algoritmo de compressão de dados de Huffman em C. Ele permite comprimir (codificar) e descomprimir (decodificar) arquivos de qualquer tipo, utilizando uma árvore de Huffman para gerar códigos binários otimizados para cada byte do arquivo.

## Observações

- O programa foi desenvolvido para fins didáticos e pode ser utilizado para comprimir arquivos de texto ou binários.
- O código possui tratamento de erros e faz a limpeza de toda a memória alocada.
- As mensagens do programa são exibidas em português.

## Como compilar

No terminal, dentro da pasta do projeto, execute:

```sh
gcc arvore.c main.c codificador.c codigo.c decodificador.c dicionario.c arquivos.c lista_encad.c tabela_freq.c -o huffman
```

## Como rodar

Após compilar, execute o programa:

```sh
./huffman
```

O programa exibirá um menu interativo:

```
Selecione uma opção:
1 - Codificar o arquivo
2 - Decodificar o arquivo
Opção:
```

Basta digitar o número da opção desejada e seguir as instruções na tela para informar os nomes dos arquivos.

## Estrutura dos arquivos

- **main.c**: Função principal e controle do fluxo do programa.
- **arvore.c**: Funções para manipulação da árvore de Huffman.
- **codificador.c**: Funções para compressão dos dados.
- **decodificador.c**: Funções para descompressão dos dados.
- **codigo.c**: Manipulação dos códigos binários.
- **dicionario.c**: Criação e liberação do dicionário de códigos.
- **arquivos.c**: Funções auxiliares para manipulação de arquivos.
- **lista_encad.c**: Implementação da lista encadeada usada na construção da árvore.
- **tabela_freq.c**: Criação da tabela de frequências dos bytes.

## Requisitos

- Compilador GCC
- Sistema operacional compatível com C (Linux, Windows, etc.)