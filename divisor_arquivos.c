/*
COMO EXECUTAR O CÓDIGO (Linux): 
Compile o código-fonte com o GCC em algum diretório, então crie um arquivo de teste nesse mesmo diretório (ou baixe o "teste.txt" anterior), escolha a opção 1 e 
informe o nome do arquivo (se o arquivo escolhido estiver em outro diretório que não seja o do código fonte, digite o caminho completo). Em seguida, informe em 
quantas partes quer dividir o arquivo, assim a divisão será feita. Para uni-lo novamente, escolha a opção 2 e informe outra vez o nome do arquivo.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <stdio_ext.h>
#define clear() printf("\033[H\033[J")

bool atribuicao_dados_divisao(int *p, bool *vd, char imprime_fim[100]);
void atribuicao_dados_uniao(int *p, char imprime_fim[100]);
void verifica_acao(int **p, char n[100], char rpos[5], char **pos, bool v);
void divide_arquivo(FILE **pfile, FILE **pcopy, char n[100], char rpos[5], int **p, unsigned long int tam, bool **vd);
void une_arquivos(FILE **pfile, FILE **ppfile, unsigned long int *s);
void altera_nome(char n[100], char **pos, char c[5], char rpos[10], int cont, int *len, bool v);
bool verifica_ponto(char n[100], char **pos);
void clear_n(char str[100]);
void zera_n(char n[100]);

int main(int argc, char *argv[]) {

    setlocale(LC_ALL, "");

    int op = 0, partes = 0, imprime_n = 0;
    char blue[] = {0x1b, '[', '1', ';', '3', '4', 'm', 0 };
    char normal[] = {0x1b, '[', '0', ';', '3', '9', 'm', 0 };
    char imprime_fim[100] = {0};
    bool verifica_divisao = false, flag = false;

    do {

        clear();
        printf("\n1 - Dividir arquivo\n2 - Unir arquivo\n3 - Sair\n\nEscolha uma opção: ");
        scanf("%d", &op);

        flag = false;
        partes = 0;
        imprime_n = 0;

        while (op < 1 || op > 3) {

            if (imprime_n == 0) printf("\n");

            printf("Inválido! Digite \"1\" para dividir um arquivo, \"2\" para unir um arquivo ou \"3\" para sair: ");
            scanf("%d", &op);
            ++imprime_n;

        }

        switch (op) {

            case 1:

                if (atribuicao_dados_divisao(&partes, &verifica_divisao, imprime_fim)) {

                    clear();
                    if (verifica_divisao == true)

                        printf("\nA divisão do arquivo por %d não deu exata! A %dª parte ficou com mais bytes!", partes, partes);

                    printf("\nArquivo %s\"%s\"%s dividido com sucesso em %d partes!\n", blue, imprime_fim, normal, partes);

                    remove(imprime_fim);
                    flag = true;

                }

                break;

            case 2:

                atribuicao_dados_uniao(&partes, imprime_fim);

                clear();
                printf("\nArquivo %s\"%s\"%s unido com sucesso em %d partes!\n", blue, imprime_fim, normal, partes);

                flag = true;
                break;

            }


            if (op != 3 && flag == true) {

                printf("\nPressione ENTER para continuar... ");
                __fpurge(stdin);
                getc(stdin);

            }


    }
    while (op != 3);

    clear();
    printf("\nSaindo!\n\n");

    return 0;

}

bool atribuicao_dados_divisao(int *p, bool *vd, char imprime_fim[100]) {

    FILE *pfile = NULL, *pcopy = NULL;
    unsigned long int tamanho = 0;
    int cont = 0;
    char blue[] = {0x1b, '[', '1', ';', '3', '4', 'm', 0 };
    char red[] = {0x1b, '[', '0', ';', '3', '1', 'm', 0 };
    char normal[] = {0x1b, '[', '0', ';', '3', '9', 'm', 0 };
    char nome[100] = {0}, rpos[5] = {0}, op;

    clear();
    printf("\nInforme o nome do arquivo a ser dividido: ");
    __fpurge(stdin);
    fgets(nome, 100, stdin);
    clear_n(nome);

    pfile = fopen(nome, "r");

    if (pfile == NULL) {

        clear();
        printf("\nErro ao abrir o arquivo %s\"%s\"%s!\n\n", red, nome, normal);
        exit(1);

    }

    fseek(pfile, 0, SEEK_END);
    tamanho = ftell(pfile);

    if (tamanho < 2) {

        printf("\nInválido! Arquivo com %lu byte de tamanho!\n\n", tamanho);
        exit(1);

    }

    printf("Informe em quantas partes quer dividir o arquivo %s\"%s\"%s: ", blue, nome, normal);
    scanf("%d", &(*p));

    while (*p <= 1 || *p > tamanho || *p > 5000) {

        if (*p <= 1) {

            printf("\nInválido! É necessário dividir o arquivo em mais de 1 parte, digite novamente: ");
            scanf("%d", &(*p));

        }

        else if (*p > tamanho) {

            printf("\nInválido! A quantidade de partes é maior que o tamanho do arquivo, digite novamente: ");
            scanf("%d", &(*p));

        }

        else {

            printf("\nInválido! O limite para a divisão de arquivos é de 5000 partes, digite novamente: ");
            scanf("%d", &(*p));

        }

    }

    printf("\nATENÇÃO! O arquivo original %s\"%s\"%s será dividido em %d partes! Tem certeza que deseja continuar (s/n)? ", blue, nome, normal, *p);
    scanf(" %c", &op);

    while (op != 's' && op != 'S' && op != 'n' && op != 'N') {

        if (cont == 0) printf("\n");

        printf("Inválido! Digite \"s\" ou \"n\": ");
        scanf(" %c", &op);

        ++cont;

    }

    if (op == 'n' || op == 'N') return false;

    tamanho /= *p;

    strcpy(imprime_fim, nome);

    rewind(pfile);

    divide_arquivo(&pfile, &pcopy, nome, rpos, &p, tamanho, &vd);

    return true;

}

void atribuicao_dados_uniao(int *p, char imprime_fim[100]) {

    char nome[100] = {0}, rpos[5] = {0}, *posicao = NULL;
    bool verifica = false;

    clear();
    printf("\nInforme o nome do arquivo a ser unido: ");
    __fpurge(stdin);
    fgets(nome, 100, stdin);
    clear_n(nome);

    strcpy(imprime_fim, nome);

    verifica = verifica_ponto(nome, &posicao);

    verifica_acao(&p, nome, rpos, &posicao, verifica);

    return;

}

void verifica_acao(int **p, char n[100], char rpos[5], char **pos, bool v) {

    FILE *pfile = NULL, *ppfile = NULL;
    char red[] = {0x1b, '[', '0', ';', '3', '1', 'm', 0 };
    char normal[] = {0x1b, '[', '0', ';', '3', '9', 'm', 0 };
    char c[5] = {0}, nome_recebe[50] = {0};
    unsigned long int soma = 0;
    int len = 0;
    bool flag = false;

    strcpy(nome_recebe, n);

    if (v) strcpy(rpos, *pos);

    while (flag == false) {

        sprintf(c, "%d", (**p) + 1);

        if (v) strtok(n, ".");

        if (**p == 0) {

            len = strlen(n);
            n[len] = '-';

        }

        for (int j = len + 1; n[j] != '\0'; ++j)

            n[j] = '\0';

        strcat(n, c);

        if (v) strcat(n, rpos);

        ppfile = fopen(n, "r");

        if (ppfile != NULL) {

            if (**p == 0) pfile = fopen(nome_recebe, "w");

            une_arquivos(&pfile, &ppfile, &soma);
            remove(n);
            ++(**p);

        }

        else flag = true;

    }

    if (**p != 0) {

        rewind(pfile);
        fclose(pfile);
        return;

    }

    else {

        clear();
        printf("\nErro ao unir o arquivo %s\"%s\"%s! O arquivo não existe ou pode não ter sido dividido previamente!\n\n", red, nome_recebe, normal);
        exit(1);

    }

}

void divide_arquivo(FILE **pfile, FILE **pcopy, char n[100], char rpos[5], int **p, unsigned long int tam, bool **vd) {

    unsigned long int soma = 0;
    int lenght = 0;
    char *ptr = NULL, *posicao = NULL, caracter[5] = {0};
    bool verifica = false;

    verifica = verifica_ponto(n, &posicao);

    if (verifica) strcpy(rpos, posicao);

    for (int i = 1; i <= **p; ++i) {

        ptr = (char*) malloc(tam * sizeof(char));

        if (ptr == NULL) {

            clear();
            printf("\nErro na alocação de memória! Saindo do programa!\n\n");
            exit(1);

        }

        altera_nome(n, &posicao, caracter, rpos, i, &lenght, verifica);

        *pcopy = fopen(n, "w");

        if (*pcopy == NULL) {

            clear();
            printf("\nErro ao criar a %dª parte do arquivo!\n\n", i);
            exit(1);

        }

        fread(ptr, sizeof(char), tam, *pfile);
        fwrite(ptr, sizeof(char), tam, *pcopy);

        soma += tam;

        fseek(*pfile, soma, SEEK_SET);

        if (i == **p) {

            zera_n(n);

            for (int j = 0; !feof(*pfile); ++j) {

                n[j] = fgetc(*pfile);

                if (n[j] == '\377') continue;

                fwrite(n, sizeof(char), 1, *pcopy);
                **vd = true;

            }

        }

        memset(ptr, '\0', tam);
        fclose(*pcopy);
        free(ptr);

    }

    rewind(*pfile);
    fclose(*pfile);

    return;

}

void une_arquivos(FILE **pfile, FILE **ppfile, unsigned long int *s) {

    char *ptr = NULL;
    unsigned long int tam = 0;

    fseek(*ppfile, 0, SEEK_END);
    tam = ftell(*ppfile);

    ptr = (char*) malloc(tam * sizeof(char));

    if (ptr == NULL) {

        clear();
        printf("\nErro na alocação de memória! Saindo do programa!\n\n");
        exit(1);

    }

    rewind(*ppfile);

    fread(ptr, sizeof(char), tam, *ppfile);
    fwrite(ptr, sizeof(char), tam, *pfile);

    *s += tam;

    fseek(*pfile, *s, SEEK_SET);

    memset(ptr, '\0', tam);
    fclose(*ppfile);
    free(ptr);

    return;

}

void altera_nome(char n[100], char **pos, char c[5], char rpos[10], int cont, int *len, bool v) {

    sprintf(c, "%d", cont);

    if (v) strtok(n, ".");

    if (cont == 1) {

        *len = strlen(n);
        n[*len] = '-';

    }

    for (int j = *len + 1; n[j] != '\0'; ++j)

        n[j] = '\0';

    strcat(n, c);

    if (v) strcat(n, rpos);

    return;

}

bool verifica_ponto(char n[100], char **pos) {

    bool flag = false;

    for (int i = 0; n[i] != '\0' && flag == false; ++i)

        if(n[i] == '.') {

            *pos = &n[i];
            flag = true;

        }

    if (flag == false) return false;

    else return true;

}

void clear_n(char str[100]) {

    for (int i = 0; str[i] != '\0'; ++i)

        if (str[i] == '\n') {

            str[i] = '\0';

        }

    return;

}

void zera_n(char n[100]) {

    for (int i = 0; i < 100; ++i)

        n[i] = '\0';

    return;

}
