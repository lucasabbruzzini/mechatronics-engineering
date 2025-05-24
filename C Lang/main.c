// Lucas Rodrigues Abbruzzini 11521emt022 lucas.abbruzzini@gmail.com
// Anderson Mesquita  11521emt015 andersonmn95@gmail.com


#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

struct livro{
    char autor[50], titulo[50];
    int edicao, volume, status, matricula;
};

int cadastrar(int mat);
void emprestar(int mat);
void alterar(int mat);
void excluir(int mat);
int pesquisa(int mat);

int main(){
    int mat, i;

    printf("Bem vindo\n");
    printf("Digite a opcao desejada:\n");
    printf("1 - Cadastrar um livro\n");
    printf("2 - Pesquisar um titulo\n");
    printf("3 - Reservar ou Emprestar um livro\n");
    printf("4 - Alterar cadastro\n");
    printf("5 - Excluir um titulo\n");
    printf("Digite 0 para Encerrar");
    scanf("%d", &i);

    do{
        switch(i){
            case '1':{
                cadastrar();
            break;
            }
            case '2':{
            pesquisa();
            break;
            }
            case '3':{
            reservar();
            break;
            }
            case '4':{
                alterar();
                break;
            }
            case '5':{
                excluir();
                break;
            }
            default:{
            printf("Escolha uma opcao valida ou 0 para encerrar");
            scanf("%d", &i);
            break;
            }
    }while(i != 0);

system("pause");
return 0;
}

int cadastrar(int mat){
    int mat, j, m;
    struct livro livro[10000];
    char procura_titulo[50];

        fflush(stdin);
        printf("\n\nTitulo do livro: ");
        scanf("%s", &livro[mat].titulo);
        fflush(stdin);
        printf("\nAutor do livro: ");
        scanf("%s", &livro[mat].autor);
        printf("\nEdicao: ");
        scanf("%d", &livro[mat].edicao);
        printf("\nVolume:  ");
        scanf("%d", &livro[mat].volume);
        livro[mat].matricula = mat;
        mat++;
        livro[mat].status = "Disponivel";
        printf("\n A Matricula do livro sera %d", livro[mat].matricula);
return mat;
}

int pesquisa(int mat){
    int mat, j, m;
    struct livro livro[10000];
    char procura_titulo[50];


     printf("Entre com o nome do Titulo: ");
    fflush(stdin);
    scanf("%s", &procura_titulo);

    for(j=0;j<mat;j++){
        if(strcmp (procura_titulo,livro[j].titulo) == 0){
            printf("\nTitulo: %s", livro[j].titulo);
            printf("\nAutor: %s", livro[j].autor);
            printf("\nEdicao: %s", livro[j].edicao);
            printf("\nVolume: %d", livro[j].volume);
            printf("\nCodigo: %d", livro[j].matricula);
            printf("\nStatus: %c", livro[j].status);
            }else{
                printf("\n\nNao encontrado.\n\n");
                }
            printf("Digite o codigo de matricula do livro: ");
            scanf("%d", &m);

            printf("\nTitulo: %s", livro[m].titulo);
            printf("\nAutor: %s", livro[m].autor);
            printf("\nEdicao: %s", livro[m].edicao);
            printf("\nVolume: %d", livro[m].volume);
            printf("\nCodigo: %d", livro[m].matricula);
            printf("\nStatus: %c", livro[m].status);
        return m;
    }

void emprestar(int mat){
    int m, emp;
    struct livro livro[10000];

    pesquisa(int mat);

            printf("Para EMPRESTAR digite 1 e para RESERVAR digite 2");
            scanf("%d", &emp);
            if  (emp == 1){
                    livro[m].status = "EMPRESTADO";
                }else{
                if(emp == 2){
                    livro[m].status = "RESERVADO";
                }else{
                    printf("Digite um valor valido");
            }
        }
return;
}

void alterar(int mat){
    int mat, j, m, alt;
    struct livro livro[10000];

    pesquisa(int mat);

                printf("\nPara alterar TITULO digite 1");
                printf("\nPara alterar AUTOR digite 2");
                printf("\nPara alterar EDICAO digite 3");
                printf("\nPara alterar VOLUME digite 4");
                printf("\nPara alterar STATUS digite 5");
                scanf("d", &alt);
                switch(alt){
                    case '1':{
                        fere(livro[m].titulo);
                        fflush(stdin);
                        printf("\nTitulo do livro: ");
                        scanf("%s", livro[m].titulo);
                        break;
                        }
                    case '2':{
                        free(livro[m].autor);
                        fflush(stdin);
                        printf("\nAutor do livro: ");
                        scanf("%s", livro[m].autor);
                        }
                        break;
                    case '3':{
                        livro[m].edicao = NULL;
                        printf("\nEdicao do livro: ");
                        scanf("%d", livro[m].edicao);
                        break;
                        }
                    case '4':{
                        livro[m].volume = NULL;
                        printf("\nVolume do livro: ");
                        scanf("%d", livro[m].volume);
                        break;
                        }
                    case '5':{
                        livro[m].status = NULL;
                        printf("\n Status do livro: ");
                        scanf("%c", livro[m].status);
                        break;
                        }
                    default:{
                        printf("\nDigite um valor valido \n");
                        }
return;
}

void excluir(mat){
    int mat, j, m, exclui;
    struct livro livro[10000];
    char procura_titulo[50];

   pesquisa(int mat);

            printf("\nTem certeza que deseja excluir esse livro? Digite:\n1-SIM \n2-NAO");
            scanf("%d", &exclui);

            if(exclui == 1){
                free(livro[m].titulo);
                free(livro[m].autor);
                livro[m].edicao = NULL;
                livro[m].volume = NULL;
                livro[m].status = NULL;
                livro[m].matricula = NULL;
            }else{
            printf("O livro nao sera deletado");
            }
return;
}
