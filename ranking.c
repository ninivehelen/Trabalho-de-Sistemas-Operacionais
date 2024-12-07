#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_ARQUIVOS 100
#define MAX_CAMINHO 1024

struct dirent *ep;
char arquivos[MAX_ARQUIVOS][MAX_CAMINHO];
int ocorrencias[MAX_ARQUIVOS] = {0};
int total_arquivos = 0;
char palavra[256];

// Função para contar ocorrências da palavra "tecnologia" em um arquivo
int contar_ocorrencias(const char *caminho_arquivo) {
    FILE *fp = fopen(caminho_arquivo, "r");
    if (!fp) {
        perror("Erro ao abrir arquivo");
        return -1;
    }

    int count = 0;
    while (fscanf(fp, "%s", palavra) != EOF) {
        if (strcmp(palavra, "tecnologia") == 0) {
            count++;
        }
    }

    fclose(fp);
    return count;
}

// Função para processar os arquivos no diretório
void verificar_arquivos(const char *caminho_diretorio) {
    DIR *dp = opendir(caminho_diretorio);
    if (!dp) {
        perror("Erro ao abrir diretório");
        return;
    }

    while ((ep = readdir(dp)) != NULL) {
        // Ignorar os diretórios "." e ".."
        if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) {
            continue;
        }

        char caminho_arquivo[MAX_CAMINHO];
        snprintf(caminho_arquivo, sizeof(caminho_arquivo), "%s/%s", caminho_diretorio, ep->d_name);

        // Verificar se o caminho do arquivo não excede o limite
        if (strlen(caminho_arquivo) >= MAX_CAMINHO) {
            printf("Caminho do arquivo muito longo: %s\n", caminho_arquivo);
            continue;
        }

        // Contar ocorrências de "tecnologia"
        int count = contar_ocorrencias(caminho_arquivo);
        if (count >= 0) {
            // Armazenar o nome do arquivo e a contagem de ocorrências
            strcpy(arquivos[total_arquivos], ep->d_name);
            ocorrencias[total_arquivos] = count;
            total_arquivos++;
        }
    }

    closedir(dp);
}
