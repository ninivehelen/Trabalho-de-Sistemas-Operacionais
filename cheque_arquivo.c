#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include "ranking.c"
#include "salvar_raking.c"

//  Struct para armazenar informações sobre os arquivos
typedef struct arquivo_info {
    char nome[300];
    time_t ultima_modificacao;
} arquivo_info;

// Função para verificar se um arquivo está na lista
int Verifica_arquivo_lista(arquivo_info *arquivos, int quantidade, const char *nome_arquivo, time_t data_tempo) {
    for (int i = 0; i < quantidade; i++) {
        if (strcmp(arquivos[i].nome, nome_arquivo) == 0) {
            // Verifica se o arquivo foi modificado
            if (arquivos[i].ultima_modificacao != data_tempo) {
                arquivos[i].ultima_modificacao = data_tempo;
                return 2; // Arquivo foi modificado
            }
            return 1; // Arquivo já existe sem alterações
        }
    }
    return 0; // Arquivo não encontrado 
}

void monitora_diretorio(const char *caminho_arquivo, const char *nome) {
    arquivo_info arquivos[1000];
    int quantidade_arquivos = 0;

    while (1) {
        struct dirent *entrada;
        struct stat infomacao_arquivo;
        DIR *diretorio = opendir(caminho_arquivo);

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

            // informações do arquivo, saber se tem arquivo
            if (stat(caminho_arquivo_completo, &infomacao_arquivo) == -1) {
                perror("Nenhum arquivo");
                continue;
            }

            // Verificar se é um arquivo regular
            if (S_ISREG(infomacao_arquivo.st_mode)) {
                int status = Verifica_arquivo_lista(arquivos, quantidade_arquivos, entrada->d_name, infomacao_arquivo.st_mtime);

                if (status == 0) {  // Novo arquivo
                    printf("Novo arquivo criado: %s\n", entrada->d_name);
                    // Adicionar novo arquivo à lista
                    strncpy(arquivos[quantidade_arquivos].nome, entrada->d_name, sizeof(arquivos[quantidade_arquivos].nome));
                    arquivos[quantidade_arquivos].ultima_modificacao = infomacao_arquivo.st_mtime;
                    quantidade_arquivos++;

                    // Imprimir ranking após adicionar um novo arquivo
                    verificar_arquivos(caminho_arquivo);  // Chama a função que ordena os arquivos
                    for (int i = 0; i < total_arquivos; i++) {
                        printf("Nova alteração\n");
                        printf("%s: %d ocorrências\n", arquivos[i].nome, ocorrencias[i]); //imprimir na tela a quantidade
                        salvar_ranking(arquivos[i].nome, ocorrencias[i]);  //Chamar função ranking
                    }

                } else if (status == 2) {  // Arquivo modificado
                    printf("Arquivo modificado: %s\n", entrada->d_name);

                    // Imprimir ranking após modificar um arquivo
                    verificar_arquivos(caminho_arquivo);  // Chama a função que ordena os valores
                    for (int i = 0; i < total_arquivos; i++) {
                        printf("Nova alteração\n");
                        printf("%s: %d ocorrências\n", arquivos[i].nome, ocorrencias[i]); //imprimir na tela a quantidade
                         salvar_ranking(arquivos[i].nome, ocorrencias[i]); //Chamar função ranking
                    }
                }
            }
        }
        
        closedir(diretorio);
        sleep(5); // Pausa antes da próxima verificação
    }
}