#include "funcoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

int gerarXAleatorio(){
    int i;
    i = rand() % 26;
    printf("chave aleatoria = %d",i);
    return i;
}

void criptografar(char *texto, int x) {
    for (int i = 0; texto[i] != '\0'; i++) {
        char c = texto[i];

        // Letras maiúsculas
        if (c >= 'A' && c <= 'Z') {
            texto[i] = ((c - 'A' + x) % 26) + 'A';
        }
        // Letras minúsculas
        else if (c >= 'a' && c <= 'z') {
            texto[i] = ((c - 'a' + x) % 26) + 'a';
        }
        // qualquer outro caractere não muda
    }
}

void salvarArquivo(const char *nome, const char *conteudo) {
    FILE *fp = fopen(nome, "w"); // w = write (sobrescreve)
    if (!fp) {
        printf("Erro ao criar arquivo de saída!\n");
        return;
    }

    fputs(conteudo, fp);
    fclose(fp);

    printf("\nArquivo '%s' salvo com sucesso!\n", nome);
}
void inicializarChave(char chave[26]) {
    for (int i = 0; i < 26; i++) {
        chave[i] = '?';   // '?' significa "desconhecida"
    }
}


// Mostra a chave no formato solicitado:
// ABCDEFG...Z
// S ? B H ? ...
void mostrarChave(const char chave[26]) {
    // Linha 1: alfabeto
    for (int i = 0; i < 26; i++) {
        putchar('A' + i);
    }
    putchar('\n');

    // Linha 2: mapeamentos (ou espaço para desconhecido)
    for (int i = 0; i < 26; i++) {
        if (chave[i] == '?' || chave[i] == '\0')
            putchar(' ');
        else
            putchar(chave[i]);
    }
    putchar('\n');
}

char* lerArquivoCompleto(const char *nome) {
    FILE *fp = fopen(nome, "r");
    if (!fp) {
        printf("Erro ao abrir arquivo!\n");
        return NULL;
    }

    // Descobre tamanho do arquivo
    fseek(fp, 0, SEEK_END);
    long tamanho = ftell(fp);
    rewind(fp);

    // Aloca memória
    char *buffer = malloc(tamanho + 1);
    if (!buffer) {
        printf("Erro de memória!\n");
        fclose(fp);
        return NULL;
    }

    // Lê o arquivo todo
    fread(buffer, 1, tamanho, fp);
    buffer[tamanho] = '\0';  // final da string

    fclose(fp);
    return buffer;
}

void mostrarEstado(const char *textoCripto, char chave[26]) {

    printf("\n===== ESTADO ATUAL DA CRIPTOANÁLISE =====\n\n");

    // 1) Texto criptografado
    printf("=== Texto criptografado ===\n");
    printf("%s\n\n", textoCripto);

    // 2) Mostrar a chave
    printf("=== Chave ===\n");
    printf("ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");

    for (int i = 0; i < 26; i++) {
        if (chave[i] == '?')
            printf(" ");
        else
            printf("%c", chave[i]);
    }
    printf("\n\n");

    // 3) Texto parcialmente decifrado
    printf("=== Texto parcialmente decifrado ===\n");

    for (int i = 0; textoCripto[i] != '\0'; i++) {
        char c = textoCripto[i];

        if (c >= 'A' && c <= 'Z') {
            int idx = c - 'A';
            if (chave[idx] != '?') {
                // letra decifrada → imprimir em verde
                printf("\033[32m%c\033[0m", chave[idx]);
            } else {
                // letra não decifrada
                printf("%c", c);
            }
        } else {
            // Não é letra
            printf("%c", c);
        }
    }

    printf("\n\n");
}

void exportarChave(char chave[26]) {
    FILE *fp = fopen("chave_final.txt", "w");

    if (!fp) {
        printf("Erro ao criar arquivo da chave!\n");
        return;
    }

    fprintf(fp, "Chave Final da Criptoanalise:\n\n");
    fprintf(fp, "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");

    for (int i = 0; i < 26; i++) {
        if (chave[i] == '?' || chave[i] == 0)
            fprintf(fp, "?");
        else
            fprintf(fp, "%c", chave[i]);
    }

    fprintf(fp, "\n");

    fclose(fp);
    printf("\nArquivo 'chave_final.txt' salvo com sucesso!\n");
}


void analiseFrequencia(const char *texto, char chave[26]) {
    int freq[26] = {0};

    // 1. contar frequência
    for (int i = 0; texto[i] != '\0'; i++) {
        char c = toupper(texto[i]);
        if (c >= 'A' && c <= 'Z') {
            freq[c - 'A']++;
        }
    }

    // 2. mostrar tabela
    printf("\n===== Análise de Frequência =====\n");
    printf("Letra | Contagem\n");
    printf("------------------\n");

    for (int i = 0; i < 26; i++) {
        if (freq[i] > 0)
            printf("  %c   |   %d\n", 'A' + i, freq[i]);
    }

    printf("\nSugestão (não aplica automaticamente):\n");
    printf("A letra mais frequente em português = 'E'\n");
    printf("Compare as contagens para inferir substituições.\n\n");
}

void atualizarChave(char chave[26], char original, char mapeada) {
    original = toupper(original);
    mapeada = toupper(mapeada);

    if (original < 'A' || original > 'Z') {
        printf("Letra original inválida!\n");
        return;
    }

    int idx = original - 'A';
    chave[idx] = mapeada;

    printf("Chave atualizada: %c -> %c\n", original, mapeada);
}
void menuPrincipal(char *textoCriptografado, char chave[26]) {
    int opcao = 0;

    while (opcao != 6) {
        printf("\n===== MENU CRIPTOANÁLISE =====\n");
        printf("1 - Estado atual da criptoanálise\n");
        printf("2 - Análise de frequência (chute)\n");
        printf("3 - Casamento exato (Shift-And, KMP ou BM)\n");
        printf("4 - Casamento aproximado (Shift-And Aproximado)\n");
        printf("5 - Alterar chave manualmente\n");
        printf("6 - Exportar chave e encerrar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        getchar(); // limpar buffer

        switch (opcao) {

            case 1:
                mostrarEstado(textoCriptografado, chave);
                break;

            case 2: {
                printf("\n=== Análise de Frequência ===\n");
                analiseFrequencia(textoCriptografado, chave);
                break;
            }

            case 3: {
                char padrao[100];
                printf("Digite o padrão para busca exata: ");
                scanf("%s", padrao);

                shiftAndExato(textoCriptografado, padrao);
                break;
            }

            case 4: {
                char padrao[100];
                int tolerancia;

                printf("Digite padrão e tolerância: ");
                scanf("%s %d", padrao, &tolerancia);

                shiftAndAprox(textoCriptografado, padrao, tolerancia);
                break;
            }

            case 5: {
                char original, mapeada;

                printf("Digite letra original e letra mapeada (ex: A S): ");
                scanf(" %c %c", &original, &mapeada);

                atualizarChave(chave, original, mapeada);
                break;
            }

            case 6:
                exportarChave(chave);
                printf("\nEncerrando...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }
    }
}

#include <string.h>

char removerAcentos(unsigned char c) {
    static const unsigned char acentuados[] =
        "áàãâäÁÀÃÂÄ"
        "éèêëÉÈÊË"
        "íìîïÍÌÎÏ"
        "óòõôöÓÒÕÔÖ"
        "úùûüÚÙÛÜ"
        "çÇ";

    static const unsigned char semAcento[] =
        "aaaaaAAAAA"
        "eeeeEEEE"
        "iiiiIIII"
        "oooooOOOOO"
        "uuuuUUUU"
        "cC";

    for (int i = 0; acentuados[i] != '\0'; i++) {
        if (c == acentuados[i])
            return semAcento[i];
    }

    return c; // não tinha acento
}
