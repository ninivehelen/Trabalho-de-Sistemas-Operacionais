#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

// Função para contar a ocorrência de uma palavra em um arquivo usando o comando `grep`
int busca_palavra_grep(const char *caminho_arquivo, const char *palavra) {
    char comando[512];
    snprintf(comando, sizeof(comando), "grep -o '%s' '%s' | wc -l", palavra, caminho_arquivo);
    
    FILE *fp = popen(comando, "r");
    if (fp == NULL) {
        perror("Erro no comando grep");
        return 0;
    }

    int count = 0;
    if (fscanf(fp, "%d", &count) != 1) {
        perror("Erro na saída do comando grep");
        count = 0;
    }

    fclose(fp);
    return count;
}