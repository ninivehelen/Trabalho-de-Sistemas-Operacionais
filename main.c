#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include "salvar_raking.c"
#include "conta_ocorrencia.c"

// Struct para armazenar informações sobre o arquivo e a contagem de ocorrências

typedef struct arquivo_informacoes {
    char nome[300];
    int ocorrencias;
    time_t ultimo_tempo; // Timestamp da ultima modificação
} arquivo_informacoes;

// Função de comparação para o qsort (ordem decrescente de contagem)
int comparacao_ranking(const void *a, const void *b) {
    return ((arquivo_informacoes *)b)->ocorrencias - ((arquivo_informacoes *)a)->ocorrencias;
}

// Função para monitorar o diretório
void monitora_diretorio(const char *caminho_arquivo, const char *palavra) {
    arquivo_informacoes ranking[1000];  // Armazena quantidade ranking
    int quantidade_arquivos = 0;

    while (1) {
        struct dirent *entrada; // Dirent propria do c 
        struct stat infomacao_arquivo; // stat propria do c 
        DIR *diretorio = opendir(caminho_arquivo); // Dir propria do c 

        if (diretorio == NULL) {
            perror("Diretório não encontrado");
            exit(EXIT_FAILURE);
        }

        printf("Verificando diretório: %s\n", caminho_arquivo);

        while ((entrada = readdir(diretorio)) != NULL) {
            // Ignorar . e ..
            if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0)
                continue;

            char caminho_arquivo_completo[512];
            snprintf(caminho_arquivo_completo, sizeof(caminho_arquivo_completo), "%s/%s", caminho_arquivo, entrada->d_name);

            // Captura a modificação
            if (stat(caminho_arquivo_completo, &infomacao_arquivo) == -1) {
                perror("Nenhum arquivo");
                continue;
            }

            // Verificar se é uma rquivo regular
            if (S_ISREG(infomacao_arquivo.st_mode)) {
                // Verifica se o arquivo foi modificado
                int modificado = 1;
                for (int i = 0; i < quantidade_arquivos; i++) {
                    if (strcmp(ranking[i].nome, entrada->d_name) == 0) {
                        if (ranking[i].ultimo_tempo == infomacao_arquivo.st_mtime) {
                            modificado = 0; // Se não tem mudança
                        } else {
                            ranking[i].ultimo_tempo = infomacao_arquivo.st_mtime; // Atualiza timestamp
                        }
                        break;
                    }
                }

                // Se é novo contar novamente as palavras no arquivo
                if (modificado) {
                    int count = busca_palavra_grep(caminho_arquivo_completo, palavra);
                    if (count > 0) {
                        //  Guarda a quantidade
                        strncpy(ranking[quantidade_arquivos].nome, entrada->d_name, sizeof(ranking[quantidade_arquivos].nome));
                        ranking[quantidade_arquivos].ocorrencias = count;
                        ranking[quantidade_arquivos].ultimo_tempo = infomacao_arquivo.st_mtime;
                        quantidade_arquivos++;
                    }
                }
            }
        }

        closedir(diretorio);

        // Mostrar em ordem Decrescente 
        qsort(ranking, quantidade_arquivos, sizeof(arquivo_informacoes), comparacao_ranking);

        // Mostrar os 10 primeiros
        printf("\nTop 10 arquivos que mais tem a palavra procurada pelo usuário '%s':\n", palavra);
        for (int i = 0; i < quantidade_arquivos && i < 10; i++) {
            printf("%d. %s - %d quantidade\n", i + 1, ranking[i].nome, ranking[i].ocorrencias);
            salvar_ranking(ranking[i].nome, ranking[i].ocorrencias);
        }
        
        
        sleep(5); // Uma pausa antes de verificar novamente
    }
}

#include <stdio.h>
#include <string.h>

// Função principal
int main() {
    char caminho_arquivo[512];
    char palavra[100];
    
    // Capturando o caminho do diretório digitado pelo usuário
    printf("Digite o caminho do diretório: ");
    fgets(caminho_arquivo, sizeof(caminho_arquivo), stdin);
    
    // Remover o caractere de nova linha, se houver
    caminho_arquivo[strcspn(caminho_arquivo, "\n")] = '\0';
    
    // Capturando a palavra digitada pelo usuário
    printf("Digite a palavra a ser pesquisada: ");
    scanf("%s", palavra);

    // Chama a função monitora diretorio que verifica se o arquivo foi alterado ou excluído e atualizar o ranking
    monitora_diretorio(caminho_arquivo, palavra);

    return 0;
}
