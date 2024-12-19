#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

// Função para buscar a palavra digitada pelo usuário usando grep
int busca_palavra_grep(const char *caminho_arquivo, const char *palavra) {
    char comando[512];
    snprintf(comando, sizeof(comando), "grep -woi '\\b%s\\b' '%s' | wc -l", palavra, caminho_arquivo);

    FILE *fp = popen(comando, "r");  // Executa o comando e pegar a saída
    if (fp == NULL) {
        perror("Erro ao executar o comando grep");
        return -1;
    }

    int count;
    fscanf(fp, "%d", &count);  // Lê a contagem do comando
    pclose(fp);

    // Exibe o nome do caminho_arquivo e a contagem
    printf("Arquivo: %s | total '%s': %d\n", caminho_arquivo, palavra, count);

    return count;  // Retorna a contagem
}
