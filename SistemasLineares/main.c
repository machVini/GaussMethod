#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef
    struct No {
        void* info;
        struct No* prox;
        struct No* ant;
    }No;

typedef //LISTA DESORDENADA
    struct Lista {
        No* inicio;
        No* ult;
        int qtd;
        char* (*toString) (void*);
        int (*equals) (void*, void*);
    }Lista;

typedef
    struct Sistema {
        int qtdIncognitas;
        Lista* listaIncogs;
        Lista* listaEquacoes;
        float** matrizCoefs;
        float* resultadosLinha;
    }Sistema;

char* toStringStr (char* str) {
    return str;
}

int equalsStr (char* a, char* b) {
    if(strcmp(a, b) == 0)
        return 1;

    return 0;
}

void desalocarMatriz (void** matriz, int tamanho) {
    int i;
    for(i = 0; i < tamanho; i++)
        free(*(matriz + i));
    free(matriz);
}

void desalocarNo (No* no) {
    if(no -> prox == NULL){
        free(no);
    }
    else{
        No* aux = no -> prox;
        desalocarNo(aux);
    }
}

void desalocarLista (Lista* lis){
    free(lis -> inicio);
    free(lis -> ult);
    free(lis);
}

void desalocarSistema(Sistema* sis) {
    desalocarLista(sis -> listaIncogs);
    desalocarLista(sis -> listaEquacoes);
    desalocarMatriz((void**)sis -> matrizCoefs, sis -> qtdIncognitas);
    free(sis -> resultadosLinha);
    free(sis);
}

Lista* iniciarListaString (){
    Lista* lis = (Lista*)malloc(sizeof(Lista));
    lis->inicio = NULL;
    lis->ult = NULL;
    lis->qtd = 0;
    lis -> equals = (int(*)(void*,void*))&equalsStr;
    lis->toString = (char*(*)(void*))&toStringStr;

    return lis;
}

int tem (Lista* lis, void* rInfo) {
    No* aux = lis -> inicio;

    while (aux != NULL){
        if(lis -> equals(aux -> info, rInfo))
            return 1;
        aux = aux -> prox;
    }

    return 0;
}


int inserir (Lista* lis, void* nInfo) {
    if(tem(lis, nInfo))
        return 0;

    if(lis -> inicio == NULL){
        lis -> inicio = (No*)malloc(sizeof(No));
        lis -> inicio -> info = nInfo;
        lis -> inicio -> prox = NULL;
        lis -> inicio -> ant = NULL;
        lis -> ult = lis -> inicio;
        lis -> qtd++;
        return 1;
    }

    No* aux = (No*)malloc(sizeof(No));
    aux -> info = nInfo;
    aux -> prox = NULL;
    aux -> ant = lis -> ult;
    lis -> ult -> prox = aux;
    lis -> ult = lis -> ult -> prox;
    lis -> qtd++;

    return 1;
}

int excluir (Lista* lis, void* rInfo) {
    No* aux = lis-> inicio;

    if(aux == NULL)
        return 0;

    if(lis -> equals(aux -> info, rInfo)){
        lis -> inicio = NULL;
        lis -> qtd--;
        return 1;
    }

    aux = aux -> prox;
    while(aux != NULL){
        if(lis -> equals(aux -> info, rInfo)){
            aux -> ant -> prox = aux -> prox;
            free(aux);
            return 1;
        }
        aux = aux -> prox;
        lis -> qtd--;
    }

    return 0;
}

void exibir (Lista* lis) {
    No* aux = lis -> inicio;

    char* ret = (char*)malloc(100000 * sizeof(char));
    strcpy(ret, "{");

    while(aux != NULL){
        strcat(ret, lis-> toString(aux -> info));
        aux = aux -> prox;
        if(aux != NULL){
            strcat(ret, ", ");
        }
    }
    strcat(ret, "}");

    printf("%s", ret);
}

void* getElemento (Lista* lis, int pos) {
    if(lis -> inicio == NULL)
        return NULL;

    void* ret;
    int count = 0;
    No* aux = lis -> inicio;
    while(aux != NULL){
        if(count == pos)
            break;
        aux = aux -> prox;
        count++;
    }

    if(count == pos){
        ret = (void*)aux -> info;
        return ret;
    }

    ret = NULL;
    return ret;

}

int getPos(Lista* lis, void* nInfo)
{
    if(!tem(lis, nInfo))
        return -1;

    int cont = 0;
    No* aux = lis->inicio;
    while(aux != NULL)
    {
        if(lis -> equals(aux -> info, nInfo))
            break;
        aux = aux -> prox;
        cont++;
    }

    return cont;
}


float** fazerComplemento (float** matriz, int i, int j, int ordem) {
int a, b, c = 0, d = 0;


    float** mComplem = (float**)malloc((ordem-1)*sizeof(float*));
    for(a = 0; a < ordem-1; a++)
        *(mComplem+a) = (float*)malloc((ordem -1)*sizeof(float));


    for(a = 0; a < ordem; a++){
        for(b = 0; b < ordem; b++){
            if(a != i -1 && b != j -1){
                *(*(mComplem + c)+ d) = *(*(matriz + a) + b);


                if(c == ordem -2){
                    c = 0;
                    d++;
                    break;
                }
                else{
                    c++;
                }

            }
            else{}
        }

        if(d == ordem -1)
            break;
    }

    return mComplem;
}


float determinante (float** matriz, int ordem){
    float* linha;
    float** mAux;
    int i;
    int j=0;
    float det = 0;
    int aux;

    if(ordem == 1){
        return *(*matriz);
    }

    linha = (float*)malloc(ordem * sizeof(float));
    for(i = 0; i < ordem; i++){
        *(linha + i) = *(*matriz + i);
    }
    i = 0;


    for(j = 0; j < ordem; j++){
        if((i + j) % 2 == 0)
            aux = 1;
        else
            aux = -1;
        mAux = fazerComplemento(matriz,i+1 , j+1,ordem);
        det += linha[j] * aux * determinante(mAux, ordem-1);

        desalocarMatriz((void**)mAux, ordem -1);
    }

    free(linha);
    return det;

}


float** matrizIcognita (float** coeficientes, float* resultados, int pos, int qtd){
    int i, j, a = 0;

    float** ret = (float**)malloc(qtd * sizeof(float*));
    for(i = 0; i < qtd; i++)
        ret[i] = (float*)malloc(qtd * sizeof(float));

    for(i = 0; i < qtd; i++){
        for(j = 0; j < qtd; j++){
            if(j == pos -1){
                ret[i][j] = resultados[a];
                a++;
            }
            else
                ret[i][j] = coeficientes[i][j];
        }
    }

    return ret;
}


float* obterResultado (Sistema* sis){
    int i;
    float detC;
    float detIncognita;
    float** aux;
    float* ret = (float*)malloc(sis -> qtdIncognitas * sizeof(float));

    detC = determinante(sis -> matrizCoefs, sis -> qtdIncognitas);
    if(detC == 0)
        return NULL;

    for(i = 0; i < sis -> qtdIncognitas; i++){
        aux = matrizIcognita(sis -> matrizCoefs, sis -> resultadosLinha, i+1, sis -> qtdIncognitas);
        detIncognita = determinante(aux, sis->qtdIncognitas);
        desalocarMatriz((void**)aux, sis -> qtdIncognitas);
        ret[i] = detIncognita/detC;
    }

    return ret;
}

void exibirSistema (Sistema* sis){
    int i;
    Lista* lisE = sis -> listaEquacoes;

    printf("\n{\n");
    for(i = 0; i < sis -> qtdIncognitas; i++)
        printf("%s\n", (char*)getElemento(lisE, i));
    printf("}");

}

void exibirResult (Sistema* sis) {
    int i;
    Lista* lis = sis -> listaIncogs;
    float* resolucao = obterResultado(sis);

    if(resolucao == NULL){
        printf("Sistema sem solucao, eh SPI ou SI");
        return;
    }

    printf("\nSolucao: ");
    for(i = 0; i < sis -> qtdIncognitas; i++){
        char * a = (char*)getElemento(lis, i);
        printf("%s = %.3f", a, resolucao[i]);

        if(i + 1 < sis -> qtdIncognitas)
            printf(", ");
    }

    printf(".\n");

}

char* lerArquivo(FILE* arq){
    fseek(arq, 0, SEEK_END);
    long int buffer_size = ftell(arq);
    fseek(arq, 0, SEEK_SET);
    char* concat = (char*)malloc(buffer_size);
    int i = 0;
    while (!feof(arq)) {
        *(concat+i) = fgetc(arq);
        i++;
    }
    *(concat+i-1) = '\0';
    return concat;
}

Lista* isolarEquacoes(char* nome, Sistema* sis){
    FILE* arq = fopen(nome, "r");
    char* texto = lerArquivo(arq);

    Lista* lis = (Lista*)malloc(sizeof(Lista));
    lis->inicio = NULL;
    lis->ult = NULL;
    lis->qtd = 0;
    lis -> equals = (int(*)(void*,void*))&equalsStr;
    lis->toString = (char*(*)(void*))&toStringStr;

    char* s = (char*)malloc(sizeof(char)*1024);
    char* equacao = (char*)malloc(sizeof(char)*1024);
    strcpy(s, texto);
    equacao = strtok(s, "\n");
    while (equacao)
    {
        inserir(lis, (void*)equacao);
        equacao = strtok(NULL, "\n");
    }
    fclose(arq);
    return lis;
}

void extrairCoeficientes(Sistema* sis, char* nome){
    sis->listaEquacoes = isolarEquacoes(nome, sis);
    sis -> qtdIncognitas = sis -> listaEquacoes -> qtd;

    Lista* lis = iniciarListaString();

    int i, j, n,
        inseriu = 0,
        posVariavel = 0,
        cont = 0,
        contN = 0,
        ehCo = 1;

    char c;
    char* equacao = (char*)malloc(sizeof(char)*1024);
    char* resultado = (char*)malloc(sizeof(char)*100);
    char* coeficiente = (char*)malloc(sizeof(char)*100);
    char* variavelCmp = (char*)malloc(sizeof(char)*100);

    char** variavel = (char**)malloc(sizeof(char*) * sis -> qtdIncognitas);
    for(i = 0; i < sis -> qtdIncognitas; i++){
        variavel[i] = (char*)malloc(sizeof(char)*100);
        variavel[i][0] = '\0';
    }

    sis->matrizCoefs = (float**)malloc(sis->listaEquacoes->qtd * sizeof(float*));
    for(i = 0; i < sis->listaEquacoes->qtd; i++)
       sis->matrizCoefs[i] = (float*)malloc(sis->listaEquacoes->qtd * sizeof(float));



    for(i = 0; i < sis -> listaEquacoes -> qtd; i++)
    {

        if(posVariavel >= sis -> qtdIncognitas)
            break;
        equacao = (char*)getElemento(sis->listaEquacoes,i);
        for(n = 0; n < strlen(equacao); n++)
        {
            c = equacao[n];

            if(c>=97 && c<=122)
            {
                variavel[posVariavel][cont] = c;
                cont++;
            }

            else if (cont && c == 32)
            {
                variavel[posVariavel][cont] = '\0';
                inseriu = inserir(lis, (void*)variavel[posVariavel]);
                cont = 0;

                if(inseriu)
                    posVariavel++;
            }
        }
    }

    sis -> listaIncogs = lis;
    cont = 0;

    sis -> resultadosLinha = (float*)malloc(sizeof(float)*sis -> qtdIncognitas);
    sis -> matrizCoefs = (float**)malloc(sizeof(float*)*sis -> qtdIncognitas);
    for(i = 0; i < sis -> qtdIncognitas; i++)
        sis -> matrizCoefs[i] = (float*)malloc(sizeof(float) * sis -> qtdIncognitas);


    for(i = 0; i < sis->qtdIncognitas; i++)
    {
        equacao = (char*)getElemento(sis->listaEquacoes,i);
        j = 0;
        ehCo = 1;
        for(n = 0; n < strlen(equacao); n++)
        {
            c = equacao[n];

                if(c >= 43 && c <= 57 && ehCo)
                {
                    coeficiente[contN] = c;
                    contN++;

                    if(equacao[n+1]>=97){
                        coeficiente[contN] = '\0';
                        contN = 0;
                    }
                }
                else if(c >= 43 && c <= 57 && !ehCo)
                {
                    resultado[contN] = c;
                    contN++;
                }

                if(n==strlen(equacao)-1)
                {
                    resultado[contN] = '\0';
                    sis->resultadosLinha[i] = strtof(resultado, NULL);
                    contN = 0;

                }
                else if(c>=97 && c<=122)
                {
                    variavelCmp[cont] = c;
                    cont++;
                }
                else if(c==61)
                    ehCo = 0;
                else
                {
                    if(ehCo && cont)
                    {
                        variavelCmp[cont] = '\0';

                        j = getPos(lis, variavelCmp);
                        sis->matrizCoefs[i][j] = strtof(coeficiente, NULL);
                        cont = 0;
                        contN = 0;
                        variavelCmp = (char*)malloc(sizeof(char)*100);
                    }
                }
            }
        }

        free(equacao);
        free(coeficiente);
        free(resultado);
        free(variavelCmp);
}


void solucionar () {
    Sistema sis;
    sis.listaEquacoes = NULL;
    sis.listaIncogs = NULL;
    sis.matrizCoefs = NULL;
    sis.qtdIncognitas = 0;

    char* nome = (char*)malloc(sizeof(char)*100);
    printf("Digite o nome do arquivo TXT com o sistema linear, para que o programa calcule-o: ");
    scanf("%s", nome);

    nome[strlen(nome)] = '\0';


    extrairCoeficientes(&sis, nome);
    printf("\n\nSistema: ");
    exibirSistema(&sis);
    exibirResult(&sis);

    desalocarSistema(&sis);
}


int main()
{
    int acabou = 0;
    int escolha =  -1;

    printf("\n-----------------SISTEMAS LINEARES DE GAUSS-----------------\n\n");

    printf("Se liga na instrucoes: \n");

    do{
       solucionar();
       printf("\nSair do programa? 1 - Sim / 0 - Nao \n");
       scanf("%d", &acabou);
       system("cls");
    }while(!acabou);

    return  0;
}
