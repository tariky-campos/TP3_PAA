#include "ShiftAnd.h"


void criptografar(char *texto, int x);
void salvarArquivo(const char *nome, const char *conteudo);
char* lerArquivoCompleto(const char *nome);
void menuPrincipal(char *textoCriptografado, char chave[26]);
void mostrarEstado(const char *textoCripto, char chave[26]);
int gerarXAleatorio();