#include <stdio.h>
#include <stdlib.h>

// Função para salvar o ranking em um txt chamado Ranking.txt
int salvar_ranking(const char *arquivo_nome, int qtd_palavra) {
    FILE *pont_arq; // File pointer
    pont_arq = fopen("Ranking.txt", "a"); // Abre arquivo e cria caso não tenha ainda 
    
    if (pont_arq == NULL) {
        printf("Erro na abertura do arquivo!\n");
        return 1;
    }
    
  // Salvando nome do arquivo e a quantidade de palavra pesquisada pelo usário em cada arquivo apenas o top 10
   fprintf(pont_arq, "%s: %i\n\n", arquivo_nome, qtd_palavra);  

    fclose(pont_arq);
    printf("Dados gravados com sucesso!\n");
    return 0;
}
