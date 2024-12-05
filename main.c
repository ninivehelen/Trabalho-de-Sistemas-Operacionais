#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
DIR *dp;
struct dirent *ep;

int abrir_arquivo(void) {

    dp = opendir("/home/ninive/Área de Trabalho/trabalho_so/pasta_arquivo");
    if (dp != NULL) {
        while ((ep = readdir(dp)) != NULL) {
            puts(ep->d_name);
        }
        closedir(dp);
        return 0;  // Sucesso
    } else {
        perror("Diretório não encontrado");
        return 1;  // Erro ao abrir o diretório
    }
}

int main(void) {
    abrir_arquivo();
    return 0;
}