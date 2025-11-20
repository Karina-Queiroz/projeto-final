#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

typedef struct reg {
    char tipo[10];
    char descricao[50];
    float valor;
    char data[12];
    char status;   // 'A' = ativo, '*' = excluído
} reg;

int tamanho(FILE*);
void cadastrar(FILE*);
void consultar(FILE*);
void gerarArquivo(FILE*);
void excluir(FILE*);

// limpar buffer do teclado
void limpa_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// leitura segura de string
void ler_string(char *s, int tam) {
    fgets(s, tam, stdin);
    s[strcspn(s, "\n")] = '\0';
}


//tamanho
int tamanho(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    return ftell(arq) / sizeof(reg);
}

//cadastrar
void cadastrar(FILE *arq) {
    reg lanc;
    char confirma;

    lanc.status = 'A';

    printf("\n=== Novo Lançamento ===\n");

    printf("Tipo (Receita/Despesa): ");
    ler_string(lanc.tipo, sizeof(lanc.tipo));

    printf("Descrição: ");
    ler_string(lanc.descricao, sizeof(lanc.descricao));

    printf("Valor (R$): ");
    scanf("%f", &lanc.valor);
    limpa_buffer();

    printf("Data (dd/mm/aaaa): ");
    ler_string(lanc.data, sizeof(lanc.data));

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

//consultar
void consultar(FILE *arq) {
    reg lanc;
    char dataBusca[12];
    int encontrados = 0;

    if (tamanho(arq) == 0) {
        printf("\nNenhum registro encontrado!\n");
        system("pause");
        return;
    }

    printf("\nDigite a data para consulta (dd/mm/aaaa): ");
    ler_string(dataBusca, sizeof(dataBusca));

    rewind(arq);
    printf("\n=== Lançamentos do dia %s ===\n", dataBusca);

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

    if (encontrados == 0)
        printf("\nNenhum lançamento encontrado para esta data.\n");
    else
        printf("\nTotal de lançamentos encontrados: %d\n", encontrados);

    system("pause");
}

//gerar
void gerarArquivo(FILE *arq) {
    char nomearq[80];
    reg contato;
    int i, total;
    char status_str[12];

    printf("\nGerar Arquivo Texto\n");
    printf("Nome do arquivo (sem extensao): ");
    ler_string(nomearq, sizeof(nomearq));
    strcat(nomearq, ".txt");

    FILE *arqtxt = fopen(nomearq, "w");
    if (!arqtxt) {
        printf("Erro ao criar arquivo texto.\n");
        system("pause");
        return;
    }

    fprintf(arqtxt, "RELATÓRIO COMPLETO DE LANÇAMENTOS\n\n");
    fprintf(arqtxt, "COD  %-10s %-30s %-12s %-12s %-10s\n",
            "TIPO", "DESCRICAO", "VALOR", "DATA", "STATUS");
    fprintf(arqtxt, "-------------------------------------------------------------------------------\n");

    total = tamanho(arq);

    for (i = 0; i < total; i++) {
        fseek(arq, i * sizeof(reg), SEEK_SET);
        fread(&contato, sizeof(reg), 1, arq);

        if (contato.status == '*')
            strcpy(status_str, "EXCLUIDO");
        else
            strcpy(status_str, "ATIVO");

        fprintf(arqtxt, "%03d %-10s %-30s %-12.2f %-12s %-10s\n",
                i + 1,
                contato.tipo,
                contato.descricao,
                contato.valor,
                contato.data,
                status_str);
    }

    fclose(arqtxt);
    printf("\nArquivo '%s' gerado com sucesso!\n", nomearq);
    system("pause");
}

//excluir
void excluir(FILE *arq) {
    reg lanc;
    int codigo, total = tamanho(arq);

    if (total == 0) {
        printf("\nNenhum registro para excluir!\n");
        system("pause");
        return;
    }

    printf("Digite o código do lançamento para excluir (1 a %d): ", total);
    scanf("%d", &codigo);
    limpa_buffer();

    if (codigo < 1 || codigo > total) {
        printf("\nCódigo inválido!\n");
        system("pause");
        return;
    }

    fseek(arq, (codigo - 1) * sizeof(reg), SEEK_SET);
    fread(&lanc, sizeof(reg), 1, arq);

    lanc.status = '*';  // Marca como excluído

    fseek(arq, (codigo - 1) * sizeof(reg), SEEK_SET);
    fwrite(&lanc, sizeof(reg), 1, arq);

    printf("\nRegistro excluído com sucesso!\n");
    system("pause");
}



int main() {
    int op;
    FILE *arq;

    if ((arq = fopen("C:\\Ling_C\\financeiro.dat", "rb+")) == NULL) {
        if ((arq = fopen("C:\\Ling_C\\financeiro.dat", "wb+")) == NULL) {
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
        limpa_buffer();

        switch (op) {
            case 1: cadastrar(arq); break;
            case 2: consultar(arq); break;
            case 3: gerarArquivo(arq); break;
            case 4: excluir(arq); break;
            case 5: fclose(arq); break;
            default: printf("Opção inválida!\n"); system("pause");
        }
    } while (op != 5);
 fclose(arq); // Fecha o arquivo
    return 0;
}

