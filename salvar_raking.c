#include <stdio.h>
#include <stdlib.h>

int salvar_ranking(const char *arquivo_nome, int qtd_palavra) {
    FILE *pont_arq; // Cria um ponteiro para o arquivo
    // Abrindo o arquivo no modo de anexação ("a"), para não sobrescrever o conteúdo toda vez
    pont_arq = fopen("Ranking.txt", "a");  // Usa "a" para adicionar dados ao final do arquivo
    
    // Verificando se o arquivo foi aberto com sucesso
    if (pont_arq == NULL) {
        printf("Erro na abertura do arquivo!");
        return 1;
    }
    
    // Usando fprintf para armazenar os dados no arquivo
    fprintf(pont_arq, "%s: %i\n", arquivo_nome, qtd_palavra);  // Adiciona ":" entre o nome e a quantidade 
    
    // Fechando o arquivo
    fclose(pont_arq);
    
    // Mensagem de confirmação
    printf("Dados gravados com sucesso!\n");
    return 0;
}
