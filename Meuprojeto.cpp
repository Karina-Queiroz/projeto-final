#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include<locale>

int tamanho(FILE*);
void cadastrar(FILE*);
void consultar(FILE*);
void gerarArquivo(FILE*);
void excluir(FILE*);

struct reg {
    char tipo[10];
    char descricao[50];
    float valor;
    char data[12];
    char status;
};
int tamanho(FILE*);
void cadastrar(FILE*);
void consultar (FILE*);
void gerararq(FILE*);
void excluir(FILE*);



int main() {

    int op;
    FILE *arq;

    if ((arq = fopen("C:\\Projeto_F\\financeiro.dat", "rb+")) == NULL) {
        if ((arq = fopen("C:\\Projeto_F\\financeiro.dat", "wb+")) == NULL) {
            printf("Falha ao abrir ou criar o arquivo!\n");
            system("pause");
            return 1;
        }
    }

    // Menu
    do {
        system("cls");
        printf("========= CONTROLE FINANCEIRO =========\n");
        printf("1. Cadastrar lancamento\n");
        printf("2. Consultar por data\n");
        printf("3. Gerar relatorio (arquivo .txt)\n");
        printf("4. Excluir lancamento\n");
        printf("5. Sair\n");
        printf("----------------------------------------------\n");
         printf("Total de registros: %d\n", tamanho(arq));
        printf("Opcao: ");
        scanf("%d", &op);
        fflush(stdin);

        switch (op) {
            case 1: cadastrar(arq); break;
            case 2: consultar(arq); break;
          //  case 3: gerarArquivo(arq); break;
           // case 4: excluir(arq); break;
           // case 5: fclose(arq); break;
            default: printf("Opção inválida!\n"); system("pause");
        }
    } while (op != 5);

    return 0;
}

//saber o total de registros
int tamanho(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    return ftell(arq) / sizeof(reg);
}



//cadastrar
void cadastrar(FILE *arq) {
    char confirma;
     reg lanc;

    lanc.status = 'A';
    fflush(stdin);

    printf("\n=== Novo Lançamento ===\n");
    printf("Tipo (Receita/Despesa): ");
    gets(lanc.tipo);
    printf("Descrição: ");
    gets(lanc.descricao);
    printf("Valor (R$): ");
    scanf("%f", &lanc.valor);
    fflush(stdin);
    printf("Data (dd/mm/aaaa): ");
    gets(lanc.data);

    printf("\nConfirma o cadastro (S/N)? ");
    scanf("%c", &confirma);

    if (toupper(confirma) == 'S') {
        fseek(arq, 0, SEEK_END);
        fwrite(&lanc, sizeof(reg), 1, arq);
        printf("\nRegistro gravado com sucesso!\n");
    } else {
        printf("\nCadastro cancelado.\n");
    }

    system("pause");
}

//consultar por data
void consultar(FILE *arq) {
    reg lanc;
    char dataBusca[12];
    int encontrados = 0;

    if (tamanho(arq) == 0) {
        printf("\nNenhum registro encontrado!\n");
        system("pause");
        return;
    }

    fflush(stdin);
    printf("\nDigite a data para consulta (dd/mm/aaaa): ");
    gets(dataBusca);

    rewind(arq);
    printf("\n=== Lançamentos na data %s ===\n", dataBusca);

    while (fread(&lanc, sizeof(reg), 1, arq) == 1) {
        if (lanc.status == 'A' && strcmp(lanc.data, dataBusca) == 0) {
            printf("\nTipo: %s\n", lanc.tipo);
            printf("Descrição: %s\n", lanc.descricao);
            printf("Valor: R$ %.2f\n", lanc.valor);
            printf("Data: %s\n", lanc.data);
            printf("-----------------------------\n");
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("\nNenhum lançamento encontrado para esta data.\n");
    } else {
        printf("\nTotal de lançamentos encontrados: %d\n", encontrados);
    }

    system("pause");}
    
  



