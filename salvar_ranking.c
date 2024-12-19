#include <stdio.h>
#include <stdlib.h>

// Função para salvar o ranking em um arquivo chamado Ranking.txt
int salvar_ranking(const char *arquivo_nome, int qtd_palavra) {
    FILE *arqu;
    arqu = fopen("ranking.txt", "a");

    if (arqu == NULL) {
        printf("Erro ao abrir o arquivo Ranking.txt\n");
        return 1;
    }

    fprintf(arqu, "%s: %i\n", arquivo_nome, qtd_palavra);
    fclose(arqu);

    printf("Ranking atualizado\n");
    return 0;
}
