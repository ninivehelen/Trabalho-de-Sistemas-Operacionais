#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include "cheque_arquivo.c"

FILE *fp; //Para abrir o arquivo
char ch[900];
DIR *dp; //  Buscar o arquivo diretorio variável
struct dirent *ep; // Sctrut de diferent 


// Função para abrir os arquivos no diretório. 
void abrir_arquivo(void) {

    const char *caminho = "/home/ninive/Área de Trabalho/trabalho_so/pasta_arquivo";
    dp = opendir(caminho);
    if (dp != NULL) {
        while ((ep = readdir(dp)) != NULL) {
            if (strcmp(ep->d_name, ".") != 0 && strcmp(ep->d_name, "..") != 0) {
                monitora_diretorio(caminho, ep->d_name);
            }
        }
        closedir(dp);
    } else {
        perror("Diretório não encontrado");
    }
}

// Função principal
int main(void) {
    abrir_arquivo();
    return 0;
}
