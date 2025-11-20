#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funcoes.h"   // coloque seu header aqui

int main() {
    srand(time(NULL));
    char nomeArquivo[100];

    printf("Insira o nome do arquivo: ");
    scanf("%99s", nomeArquivo);

    // 1. Ler texto original
    char *texto = lerArquivoCompleto(nomeArquivo);
    

    // 2. Gerar x, criptografar e salvar
    int x = gerarXAleatorio();
    criptografar(texto, x);
    salvarArquivo("criptografado.txt", texto);

    // 3. Carregar texto criptografado para análise
    char chave[26];
    inicializarChave(chave);
    mostrarChave(chave);

    char *textoCriptografado = lerArquivoCompleto("criptografado.txt");
    menuPrincipal(textoCriptografado, chave);

    free(texto);
    return 0;
}