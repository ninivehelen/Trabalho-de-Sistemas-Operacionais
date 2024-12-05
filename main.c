#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

FILE *fp; //Para abrir o arquivo
int qt = 0; // Contador da palavra pesquisada
char ch[900];
DIR *dp; //  Buscar o arquivo diretorio variável
struct dirent *ep; // Sctrut de diferent 

char palavra[1000][900];

// Função para verificar quantas vezes a palavra tecnologia aparece no arquivo.
void verificar_letra_arquivo(const char *caminho, const char *nome) {
    
    // Isso faz a junção do caminho do diretório mais o nome do arquivo aberto na funçao abrir aquivo
    char caminho_completo[1000];
    snprintf(caminho_completo, sizeof(caminho_completo), "%s/%s", caminho, nome);

    // Abre o arquivo
    if ((fp = fopen(caminho_completo, "r")) == NULL) {
        printf("Erro ao abrir arquivo: %s\n", caminho_completo);
        return;
    }

    printf("Arquivo aberto: %s\n", nome);
    
    // Ler o arquivo e contar as ocorrências de "tecnologia"
    while (fscanf(fp, "%s", ch) != EOF) {
        if (strcmp(ch, "tecnologia") == 0) {
            qt++;
        }
    }

    //Imprimir quanta vezes a palavra aparece em cada arquivo. 
    //printf("A palavra 'tecnologia' foi encontrada %d vez(es) no texto.\n\n", qt);
    
    // Função ranking em desenvolvimento ainda
    ranking(nome,qt);
    fclose(fp);
    
}

//Falta terminar essa função (Pensando como vou fazer ainda)
void ranking(const char *nome, int qt){
   printf("A palavra 'tecnologia' foi encontrada %d vez(es) no texto.\n\n", qt);

}

// Função para abrir os arquivos no diretório. 
void abrir_arquivo(void) {
    const char *caminho = "/home/ninive/Área de Trabalho/trabalho_so/pasta_arquivo";
    dp = opendir(caminho);
    if (dp != NULL) {
        while ((ep = readdir(dp)) != NULL) {
            if (strcmp(ep->d_name, ".") != 0 && strcmp(ep->d_name, "..") != 0) {
                verificar_letra_arquivo(caminho, ep->d_name);
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
