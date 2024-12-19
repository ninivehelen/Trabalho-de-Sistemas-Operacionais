# Atividade 2 - Paralegrep

## Descrição do Projeto

A Atividade 2 da matéria de Sistemas Operacionais é um sistema que monitora um diretório, em que o usuário digira o caminho para monitorar os arquivos editados e os novos arquivos salvos no diretório. O usuário pode fornecer o caminho do diretório e uma palavra para verificar se esta palavra está nos arquivos do diretório. Caso a palavra seja encontrada, o sistema apresenta um ranking com os 10 arquivos que mais contêm a palavra, em ordem decrescente.

## Objetivo

O objetivo do projeto é implementar o uso de threads para gerenciar tarefas simultâneas.

## Como o Código Está Dividido

O código está dividido em três arquivos `.c`:

- **main.c**: Arquivo principal, responsável por chamar as funções dos outros dois arquivos para compilar o programa. Contém as structs para gerenciar os dados, variáveis globais e as funções `monitora_diretorio`, `atualiza_diretorio`, entre outras. Também contém a thread "despachante" e a thread de "ranking".
  
- **conta_ocorrencia.c**: Contém a função `grep`, que busca a palavra digitada pelo usuário nos arquivos do diretório monitorado.
  
- **salvar_ranking.c**: Contém a função para gerenciar o ranking e salvar o ranking em um arquivo com a extensão `.txt`.

O arquivo **main.c** é o mais "complexo", pois é nele que se gerenciam as tarefas de monitoramento e de geração de ranking.

## Como Compilar o Código no Linux
* Abra a pasta que contém os códigos no terminal Linux.
* Atualize o GCC com o seguinte comando:

   ```bash
   sudo apt update
   sudo apt install gcc
     ```
Após atualizar vamos compilar o programa.
Dentro da pasta dos códigos terminal digite:

   ```bash
   gcc -o main  main.c
   ./main 
   
```
A seguir o código vai começar a compilar pedindo o caminho do diretório que você deseja monitora e a palavra que você deseja buscar nos arquivos que estão neste diretório.
Exemplo de teste

* Digite o caminho do diretorio: caminho/usuário/nomedapasta. 
* Digite a palavra buscada: tecnologia

O caminho e a palavra pode ser algum diretório que você tenha arquivos nele e deseja monitorar no seu computador, a palavra pode ser a que você quer verificar se existe nos arquivos dentro do diretório buscado.

# Desafio durante o projeto
Não foi possível implementar as 10
threads operárias, pois estavam acontecendo erros de seguimento.
Foi possível implementa apenas 2 threads, 1 para monitorar o diretório e outra para o ranking.