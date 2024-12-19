#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "salvar_ranking.c"
#include "conta_ocorrencia.c"

// Struct para armazenar informações sobre o arquivo e a contagem de ocorrências
typedef struct arquivo_informacoes {
    char nome[300];
    int ocorrencias;
    time_t ultimo_tempo; // Timestamp da última modificação
} arquivo_informacoes;

// Struct para os dados do caminho e palavra
typedef struct {
    char palavra[100];
    char caminho_arquivo[520];
} arquivo_dados;

// Variáveis globais
#define total_arquivos 10000
arquivo_informacoes ranking[total_arquivos]; // Ranking de arquivos
int quantidade_arquivos = 0;               // Número de arquivos no ranking
pthread_mutex_t ranking_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger o ranking

// Função para atualizar o ranking
void atualizar_ranking(arquivo_informacoes *info) {
    pthread_mutex_lock(&ranking_mutex);
    ranking[quantidade_arquivos] = *info;
    quantidade_arquivos++;
    pthread_mutex_unlock(&ranking_mutex);
}

// Função de comparação para o qsort (ordem decrescente de contagem)
int comparacao_ranking(const void *a, const void *b) {
    return ((arquivo_informacoes *)b)->ocorrencias - ((arquivo_informacoes *)a)->ocorrencias;
}

// Função de monitoramento do diretório
void *monitora_diretorio(void *arg) {
    arquivo_dados *arquivo_dado = (arquivo_dados *)arg;
    const char *caminho_arquivo = arquivo_dado->caminho_arquivo;
    const char *palavra = arquivo_dado->palavra;

    while (1) {
        struct dirent *entrada;
        struct stat infomacao_arquivo;
        DIR *diretorio = opendir(caminho_arquivo);
        if (diretorio == NULL) {
            perror("Erro ao abrir o diretório");
            exit(EXIT_FAILURE);
        }

        while ((entrada = readdir(diretorio)) != NULL) {
            if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0)
                continue;

            char caminho_arquivo_completo[2048];
            snprintf(caminho_arquivo_completo, sizeof(caminho_arquivo_completo), "%s/%s", caminho_arquivo, entrada->d_name);

            if (stat(caminho_arquivo_completo, &infomacao_arquivo) == -1) {
                perror("Erro ao obter informações do arquivo");
                continue;
            }

            if (S_ISREG(infomacao_arquivo.st_mode)) {
                pthread_mutex_lock(&ranking_mutex);

                int encontrado = 0;
                for (int i = 0; i < quantidade_arquivos; i++) {
                    if (strcmp(ranking[i].nome, entrada->d_name) == 0) {
                        encontrado = 1;

                        // Atualizar apenas se o arquivo foi modificado
                        if (ranking[i].ultimo_tempo != infomacao_arquivo.st_mtime) {
                            int count = busca_palavra_grep(caminho_arquivo_completo, palavra);
                            ranking[i].ocorrencias = count;
                            ranking[i].ultimo_tempo = infomacao_arquivo.st_mtime;
                        }
                        break;
                    }
                }

                if (!encontrado) {
                    // Arquivo novo, adicionar ao ranking
                    int count = busca_palavra_grep(caminho_arquivo_completo, palavra);
                    if (count > 0) {
                        arquivo_informacoes info;
                        strncpy(info.nome, entrada->d_name, sizeof(info.nome));
                        info.ocorrencias = count;
                        info.ultimo_tempo = infomacao_arquivo.st_mtime;

                        if (quantidade_arquivos < total_arquivos) {
                            ranking[quantidade_arquivos] = info;
                            quantidade_arquivos++;
                        }
                    }
                }

                pthread_mutex_unlock(&ranking_mutex);
            }
        }

        closedir(diretorio);
        sleep(5);
    }
}

// Função para processar, exibir e salvar o ranking
void *processa_ranking(void *arg) {
    const char *palavra = (const char *)arg;

    while (1) {
        pthread_mutex_lock(&ranking_mutex);

        // Ordenar o ranking
        qsort(ranking, quantidade_arquivos, sizeof(arquivo_informacoes), comparacao_ranking);

        // Exibir os 10 melhores
        printf("\nTop 10 arquivos com a palavra '%s':\n", palavra);

        // Abrir arquivo para salvar ranking
        FILE *ranking_arquivo = fopen("ranking.txt", "w");
        if (ranking_arquivo == NULL) {
            perror("Erro ao abrir o arquivo ranking.txt");
            pthread_mutex_unlock(&ranking_mutex);
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < quantidade_arquivos && i < 10; i++) {
            printf("%d. %s - %d Total\n", i + 1, ranking[i].nome, ranking[i].ocorrencias);
            fprintf(ranking_arquivo, "%d. %s - %d Total\n", i + 1, ranking[i].nome, ranking[i].ocorrencias);
            salvar_ranking(ranking[i].nome, ranking[i].ocorrencias); // Função  para salvar no formato desejado
        }

        fclose(ranking_arquivo); // Fechar o arquivo após salvar
        pthread_mutex_unlock(&ranking_mutex);

        sleep(5);
    }
}

// Função principal
int main() {
    arquivo_dados arquivo_dado;

    // Entrada do usuário para o caminho do diretório
    printf("Digite o caminho do diretório: ");
    fgets(arquivo_dado.caminho_arquivo, sizeof(arquivo_dado.caminho_arquivo), stdin);
    arquivo_dado.caminho_arquivo[strcspn(arquivo_dado.caminho_arquivo, "\n")] = '\0';

    // Entrada do usuário para a palavra a ser pesquisada
    printf("Digite a palavra a ser pesquisada: ");
    scanf("%s", arquivo_dado.palavra);

    // Criação das 2 threads, monitora diretorio e a de ranking
    pthread_t thread_monitora, thread_ranking;

    pthread_create(&thread_monitora, NULL, monitora_diretorio, (void *)&arquivo_dado);
    pthread_create(&thread_ranking, NULL, processa_ranking, (void *)arquivo_dado.palavra);

    // Aguardar threads
    pthread_join(thread_monitora, NULL);
    pthread_join(thread_ranking, NULL);

    return 0;
}
