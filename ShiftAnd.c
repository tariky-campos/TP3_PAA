#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "ShiftAnd.h"
#define MAXCHAR 256
#define MAX_M_BITS 64    // máximo de bits que suportamos no padrão
#define MAX_ERROS 10
// =========================================================
// SHIFT–AND APROXIMADO (somente substituição)
// =========================================================
#define MAX_ERROS 10

void shiftAndAprox(const char *T, const char *P, int k) {
    long n = (long) strlen(T);
    long m = (long) strlen(P);

    if (m == 0 || n == 0) {
        printf("Texto ou padrao vazio.\n");
        return;
    }
    if (m > MAX_M_BITS) {
        printf("Erro: padrao muito grande para Shift-And (max %d chars)\n", MAX_M_BITS);
        return;
    }
    if (k < 0) {
        printf("Erro: tolerancia negativa.\n");
        return;
    }
    if (k > MAX_ERROS) {
        printf("Aviso: k > MAX_ERROS (%d). Usando %d.\n", MAX_ERROS, MAX_ERROS);
        k = MAX_ERROS;
    }
    if (k > m) k = (int)m;

    // máscaras por caractere (uint64_t para suportar até 64 bits)
    uint64_t Masc[MAXCHAR];
    uint64_t R[MAX_ERROS + 1];

    // inicializa
    for (int i = 0; i < MAXCHAR; ++i) Masc[i] = 0;
    for (int j = 0; j <= MAX_ERROS; ++j) R[j] = 0;

    // construir máscaras: bit 0 corresponde a última posição do padrão (check abaixo)
    // seguí sua convenção: posição de bit = (m - i)
    for (int i = 1; i <= m; ++i) {
        unsigned char c = (unsigned char) P[i - 1];
        Masc[c] |= (1ULL << (m - i));
    }

    uint64_t Ri = (1ULL << (m - 1));
    // inicializa R[0]...R[k]
    R[0] = 0;
    for (int j = 1; j <= k; ++j) {
        R[j] = (1ULL << (m - j)) | R[j - 1];
    }

    // varre o texto
    for (long i = 0; i < n; ++i) {
        uint64_t Rant = R[0];
        uint64_t Rnovo = ((((uint64_t) Rant) >> 1) | Ri) & Masc[(unsigned char) T[i]];
        R[0] = Rnovo;

        for (int j = 1; j <= k; ++j) {
            uint64_t tmp = Rnovo;
            Rnovo = ((((uint64_t) R[j]) >> 1) & Masc[(unsigned char) T[i]])
                    | Rant
                    | (((uint64_t)(Rant | tmp)) >> 1);
            Rant = R[j];
            R[j] = Rnovo | Ri;
        }

        // se o bit 0 está ligado, houve casamento com até k erros
        if ((Rnovo & 1ULL) != 0ULL) {
            long pos = i + 1; // sua versão imprimia i+1; Ziviani usa i-m+2 em exato
            printf("Casamento aproximado (k=%d) na posicao %ld\n", k, pos);
        }
    }
}

// ================================
// SHIFT-AND EXATO
// ================================
void shiftAndExato(const char *T, const char *P) {
    long n = (long) strlen(T);
    long m = (long) strlen(P);

    if (m == 0 || n == 0) {
        printf("Texto ou padrao vazio.\n");
        return;
    }
    if (m > MAX_M_BITS) {
        printf("Erro: padrao muito grande para Shift-And (max %d chars)\n", MAX_M_BITS);
        return;
    }

    uint64_t Masc[MAXCHAR];
    uint64_t R = 0;

    for (int i = 0; i < MAXCHAR; ++i) Masc[i] = 0;

    for (int i = 1; i <= m; ++i) {
        unsigned char c = (unsigned char) P[i - 1];
        
        // 1. Marca a letra exata que o usuário digitou
        Masc[c] |= (1ULL << (m - i));

        // 2. Marca também a versão "irmã" (se digitou 'a', marca também 'A')
        if (c >= 'a' && c <= 'z') {
            Masc[toupper(c)] |= (1ULL << (m - i)); // Marca a maiúscula correspondente
        } else if (c >= 'A' && c <= 'Z') {
            Masc[tolower(c)] |= (1ULL << (m - i)); // Marca a minúscula correspondente
        }
    }
uint64_t bitFim = 1ULL << (m - 1);
    int quantidade = 0;
    for (long i = 0; i < n; ++i) {
        R = (((uint64_t) R) >> 1 | (1ULL << (m - 1))) & Masc[(unsigned char) T[i]];
        // Quando o bit 0 está ligado significa match completo do padrão
        if ((R & 1ULL) != 0ULL) {
            quantidade++;
            long inicio = i - m + 2; // posição inicial (1-based, compatível com saída anterior)
            printf("Casamento na posicao %ld\n", inicio);
        }
    }
    if (quantidade == 0) {
        printf("Nenhum casamento encontrado.\n");
    }else{
        printf("Total de casamentos encontrados: %d\n", quantidade);
    }
}