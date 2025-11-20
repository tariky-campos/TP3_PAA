#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ShiftAnd.h"
#define MAXCHAR 256

// =========================================================
// SHIFT–AND APROXIMADO (somente substituição)
// =========================================================
#define MAX_ERROS 10

void shiftAndAproximado(const char *T, const char *P, int k) {
    long n = strlen(T);
    long m = strlen(P);

    long Masc[MAXCHAR];
    long R[MAX_ERROS + 1];

    long Ri, Rant, Rnovo;

    for (int i = 0; i < MAXCHAR; i++)
        Masc[i] = 0;

    for (int i = 1; i <= m; i++)
        Masc[(unsigned char)P[i-1]] |= 1 << (m - i);

    R[0] = 0;
    Ri = 1 << (m - 1);

    for (int j = 1; j <= k; j++)
        R[j] = (1 << (m - j)) | R[j - 1];

    for (int i = 0; i < n; i++) {

        Rant = R[0];
        Rnovo = ((((unsigned long)Rant) >> 1) | Ri)
                & Masc[(unsigned char)T[i]];

        R[0] = Rnovo;

        for (int j = 1; j <= k; j++) {

            long temp = Rnovo;

            Rnovo = ((((unsigned long)R[j]) >> 1)
                      & Masc[(unsigned char)T[i]])
                      | Rant
                      | (((unsigned long)(Rant | temp)) >> 1);

            Rant = R[j];
            R[j] = Rnovo | Ri;
        }

        if ((Rnovo & 1) != 0) {
            printf("Casamento (k=%d) na posicao %d\n", k, i + 1);
        }
    }
}

// ================================
// SHIFT-AND EXATO
// ================================
void shiftAndExato(const char *T, const char *P) {
    long n = strlen(T);
    long m = strlen(P);
    long Masc[MAXCHAR];
    long R = 0;

    for (int i = 0; i < MAXCHAR; i++)
        Masc[i] = 0;

    for (int i = 1; i <= m; i++)
        Masc[(unsigned char)P[i-1]] |= 1 << (m - i);

    for (int i = 0; i < n; i++) {
        R = ((unsigned long)R >> 1 | (1 << (m - 1)))
            & Masc[(unsigned char)T[i]];

        if ((R & 1) != 0) {
            printf("Casamento na posicao %ld\n", i - m + 2);
        }
    }
}