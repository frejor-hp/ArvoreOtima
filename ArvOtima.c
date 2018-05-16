#include <stdio.h>
#include <stdlib.h>

typedef struct _ARVORE{
    int chave;
    int k;
    struct _ARVORE *esq;
    struct _ARVORE *dir;
}ARVORE;

typedef struct _TABELA{
    int frequencia;
    int custo;
    int k;
}TABELA;

void recebeValores(int **chaves, int **f, int **fl, int n);
void declaraTabela(TABELA ***tabela, const int *f, const int *fl, int n);
void declaraDiagPrin(TABELA ***tabela, const int *fl, int n);
void criaArvore(ARVORE **arvOtima, TABELA **tabela, int *chaves, int i, int j);
void printaTabela(TABELA **tabela, int n);
void printaArvore(ARVORE *arvOtima);
void libera(int **chaves, int **f, int **fl, TABELA ***tabela, int n);
void liberaArvore(ARVORE **arvOtima);

int main() {

    ARVORE *arvOtima;
    TABELA **tabela;
    int n;
    int *chaves = 0;
    int *f = 0;
    int *fl = 0;

    arvOtima = (ARVORE*) malloc(sizeof(ARVORE));
    arvOtima->dir = NULL;
    arvOtima->esq = NULL;

    scanf("%d", &n);

    recebeValores(&chaves, &f, &fl, n);

    declaraTabela(&tabela, f, fl, n);

    /*arvOtima->frequencia = tabela[0][n].frequencia;
    arvOtima->custo = tabela[0][n].custo;
    arvOtima->k = tabela[0][n].k;
    arvOtima->chave = chaves[arvOtima->k];*/

    criaArvore(&arvOtima, tabela, chaves, 0, n);

    //printaTabela(tabela, n);

	printaArvore(arvOtima);

    printf("\n");

    libera(&chaves, &f, &fl, &tabela, n);

    return 0;
}

void recebeValores(int **chaves, int **f, int **fl, int n){
    int i;

    *chaves = (int *) malloc(n * sizeof(int));
    *f = (int *) malloc(n * sizeof(int));
    *fl = (int *) malloc((n + 1) * sizeof(int));

    for(i = 0; i < n; i++){
        scanf("%d", &(*chaves)[i]);
    }
    for(i = 0; i < n; i++){
        scanf("%d", &(*f)[i]);
    }
    for(i = 0; i < (n+1); i++){
        scanf("%d", &(*fl)[i]);
    }
}

void declaraTabela(TABELA ***tabela, const int *f, const int *fl, int n){

    int i, j, k, custo, aux;
    int melhorK;
    int menorCusto;
    TABELA **tabelaAux;

    *tabela = (TABELA**) malloc((n+1) * sizeof(TABELA*));

    for(i = 0; i < (n+1); i ++){
        (*tabela)[i] = (TABELA*) malloc((n+1) * sizeof(TABELA));
    }

    tabelaAux = *tabela;

    declaraDiagPrin(tabela, fl, n);

    for(i = 0; i < n; i++){
        for(j = (i+1); j < (n+1); j++){
            tabelaAux[i][j].frequencia = tabelaAux[i][j-1].frequencia + f[j-1] + fl[j];
        }
    }

    for(i = 0, j = 1; i < n; i++, j++){
        melhorK = j;
        tabelaAux[i][j].k = melhorK;
        tabelaAux[i][j].custo = tabelaAux[i][melhorK-1].custo + tabelaAux[melhorK][j].custo + tabelaAux[i][j].frequencia;
    }

    for(aux = 2; aux < (n+1); aux++){
        for(i = 0, j = aux; i < (n+1-aux); i++, j++){
            melhorK = tabelaAux[i][j-1].k;
            menorCusto = tabelaAux[i][melhorK-1].custo + tabelaAux[melhorK][j].custo + tabelaAux[i][j].frequencia;
            for(k = melhorK + 1; k <= tabelaAux[i+1][j].k; k++) {
                custo = tabelaAux[i][k-1].custo + tabelaAux[k][j].custo + tabelaAux[i][j].frequencia;
                if(menorCusto >= custo){
                    menorCusto = custo;
                    melhorK = k;
                }
            }
            tabelaAux[i][j].k = melhorK;
            tabelaAux[i][j].custo = menorCusto;
        }
    }
}

void declaraDiagPrin(TABELA ***tabela, const int *fl, int n){
    int i, j;

    for (i = 0, j = 0; i < (n+1); i++, j++) {
        (*tabela)[i][j].frequencia = fl[i];
        (*tabela)[i][j].custo = 0;
        (*tabela)[i][j].k = 0;
    }
}

void criaArvore(ARVORE **arvOtima, TABELA **tabela, int *chaves, int i, int j){
    ARVORE *auxArv = *arvOtima;
    ARVORE *novo;

	auxArv->k = tabela[i][j].k;
	auxArv->chave = chaves[auxArv->k-1];
	auxArv->esq = NULL;
	auxArv->dir= NULL;

	if((tabela[i][auxArv->k-1].k < auxArv->k) && (tabela[i][auxArv->k-1].k != 0)){
		novo = (ARVORE *) malloc(sizeof(ARVORE));
		auxArv->esq = novo;
		criaArvore(&novo, tabela, chaves, i, (auxArv->k-1));
	}
	if((tabela[auxArv->k][j].k > auxArv->k) && (tabela[auxArv->k][j].k != 0)){
		novo = (ARVORE *) malloc(sizeof(ARVORE));
		auxArv->dir = novo;
		criaArvore(&novo, tabela, chaves, auxArv->k, j);
	}
}

void printaTabela(TABELA **tabela, int n){

    int i, j;

    for(i = 0; i < (n+1); i++){
        for(j = i; j < (n+1); j++){
            printf("%d  --  ", tabela[i][j].k);
        }
        printf("\n");
    }
}

void printaArvore(ARVORE *arvOtima){
	printf("%d  ", arvOtima->chave);

	if(arvOtima->esq != NULL){
		printaArvore(arvOtima->esq);
	}
	if(arvOtima->dir != NULL){
		printaArvore(arvOtima->dir);
	}
}

void libera(int **chaves, int **f, int **fl, TABELA ***tabela, int n){
    int i;

    free(*chaves);
    free(*f);
    free(*fl);
    for(i = 0; i < n+1; i++){
        free((*tabela)[i]);
    }
    free(*tabela);
}

void liberaArvore(ARVORE **arvOtima){
    if((*arvOtima)->esq != NULL){
        liberaArvore(&(*arvOtima)->esq);
    }
    if((*arvOtima)->dir != NULL){
        liberaArvore(&(*arvOtima)->dir);
    }

    free(*arvOtima);
}
