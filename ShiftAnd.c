#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ShiftAnd.h"

// =========================================================
// SHIFT–AND APROXIMADO (somente substituição)
// =========================================================
void shiftAndAproximado(const char *texto, const char *padrao, int maxErros) {
    int n = strlen(texto);
    int m = strlen(padrao);

    if (m > 63) {
        printf("Erro: padrão muito grande para Shift-And (máx 63 chars)\n");
        return;
    }

    if (maxErros < 0 || maxErros > m) {
        printf("Erro: tolerância inválida.\n");
        return;
    }

    uint64_t mask[256];
    uint64_t D[64];     // D[e] → estado para e erros (0 ≤ e ≤ maxErros)

    // Inicializa máscaras
    for (int i = 0; i < 256; i++)
        mask[i] = 0;

    // Constroi máscaras do padrão
    for (int i = 0; i < m; i++) {
        unsigned char c = padrao[i];
        mask[c] |= (1ULL << i);
    }

    // Inicializa estados
    for (int e = 0; e <= maxErros; e++)
        D[e] = 0;

    uint64_t bitFim = 1ULL << (m - 1);

    int ocorrencias = 0;

    // =====================================================
    // Varre o texto
    // =====================================================
    for (int i = 0; i < n; i++) {
        unsigned char c = texto[i];

        uint64_t anteriorD0 = D[0];

        // ---- sem erro ----
        D[0] = ((D[0] << 1) | 1ULL) & mask[c];

        // ---- com erros (somente substituição) ----
        for (int e = 1; e <= maxErros; e++) {
            uint64_t anterior = D[e];

            // Caso de substituição: (D[e] << 1) | 1  
            // OU estado anterior D[e-1] (que adiciona 1 erro)
            D[e] = ((D[e] << 1) | 1ULL) & mask[c];
            D[e] |= (anteriorD0 << 1) | 1ULL;

            anteriorD0 = anterior;
        }

        // Verifica matches com até maxErros
        for (int e = 0; e <= maxErros; e++) {
            if (D[e] & bitFim) {
                int inicio = i - m + 1;

                if (inicio >= 0) {
                    ocorrencias++;
                    printf("\n↳ Ocorrência (erro = %d) na posição %d:\n", e, inicio);

                    // imprime a substring correspondente
                    printf("   Trecho: ");
                    for (int j = 0; j < m; j++)
                        printf("%c", texto[inicio + j]);
                    printf("\n");
                }
            }
        }
    }

    printf("\nTotal de ocorrências aproximadas: %d\n", ocorrencias);
}
// ================================
// SHIFT-AND EXATO
// ================================
void shiftAndExato(const char *texto, const char *padrao) {
    int n = strlen(texto);
    int m = strlen(padrao);

    if (m > 63) {
        printf("Erro: padrão muito grande para Shift-And (limite = 63 caracteres)\n");
        return;
    }

    uint64_t mask[256];   // tabela de máscaras
    uint64_t R = 0;       // registrador
    uint64_t bitFim = 1ULL << (m - 1);  // posição que identifica um match

    // Inicializa todas as máscaras com 0
    for (int i = 0; i < 256; i++)
        mask[i] = 0;

    // Preenche a máscara do padrão
    for (int i = 0; i < m; i++) {
        unsigned char c = padrao[i];
        mask[c] |= (1ULL << i);
    }

    // Procura no texto
    int ocorrencias = 0;

    for (int i = 0; i < n; i++) {
        unsigned char c = texto[i];

        R = ((R << 1) | 1ULL) & mask[c];

        if (R & bitFim) {
            ocorrencias++;
            printf("↳ Padrão encontrado na posição %d\n", i - m + 1);
        }
    }

    printf("\nTotal de ocorrências: %d\n", ocorrencias);
}