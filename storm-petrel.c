//João Victor Bravo, Matheus Farias, Tiago Valença

#include <stdio.h>
#include <stdlib.h>
#define ordem 5
/*a ordem da árvore só pode ser alterada
antes do programa ser iniciado*/

typedef struct bnode {
    int num_chaves; //numero de chaves no nó
    int chaves[ordem-1]; //array de chaves que contem os valores inseridos no nó
    struct bnode *filhos[ordem]; //array de ponteiros para os filhos do nó
}arvore;

arvore *root = NULL; //criação do nó raiz que começa como NULL

//protótipo das funções que vão ser usadas no código
void inserir(int chave);
void display(arvore *root, int espacos);
int validarInsercao(arvore *r, int x, int* y, arvore** u);
int procurarPos(int x,int *chaves, int num_chaves);

int main()
{
    int chave;//chave que irá ser adicionada
    int escolha;//variavel para escolher dentre as funcionalidades do programa
    printf("Criação de uma árvore B de Ordem = %d\n",ordem);
    while(1)
    {
        printf("\n");
        printf("1.Inserir\n");
        printf("2.Mostrar a árvore\n");
        printf("3.Sair\n");
        printf("Digite a sua escolha : ");
        scanf("%d",&escolha);

        switch(escolha) //switch que recebe a variavel escolha
        {
        case 1: //no caso 1, o programa pede para o usuário digitar uma chave e chama a função inserir
            printf("Digite a chave : ");
            scanf("%d",&chave);
            inserir(chave);
            break; //break para sair do switch e voltar para o menu

        case 2: //no caso 2 ele chama a função display que mostra a árvore B
            printf("árvore B:\n");
            display(root,0);
            break; //break para sair do switch e voltar para o menu

        case 3:
            exit(1); //fecha o programa

        default: //caso qualquer outro valor for digitado a escolha é inválida
            printf("Faça uma escolha válida\n");
            break; //break para sair do switch e voltar para o menu
        }
    }
    return 0;
}

void inserir(int chave)//a função inserir recebe um valor inteiro "chave"
{
    arvore *newnode;
    /*criação de um novo nó que poderá ser utilizado caso o numero
    de valores no nó raiz após a insercao seja maior que ordem-1*/

    int chaveAadd; //chave que será adicionada ao final da função inserir
    int flag; //uma Flag que indica se o valor da chave já existe na árvore ou não

    flag = validarInsercao(root, chave, &chaveAadd, &newnode);
    //a flag vai receber o valor da funçao validarInsercao
    if (!flag)  printf("Chave já existe\n");
    //se a flag for falsa a chave já existe na árvore
  
    if (flag == 1)
    /*se a flag for 1 significa que ocorreu o split no nó anterior então
    deveremos criar um novo nó pai*/
    {
        arvore *filhoE = root;
        //criamos um nó filhoE que vai ser igual ao no raiz que foi alterado na função validarInsercao

        root = malloc(sizeof(arvore)); //aloca o espaço de memória para o nó raiz

        root->num_chaves = 1; //o nó raiz vai ter 1 valor que subiu no split

        root->chaves[0] = chaveAadd; //o valor que vai subir no split é a chaveAadd

        root->filhos[0] = filhoE; //o primeiro ponteiro do array de filhos aponta para o filhoE

        root->filhos[1] = newnode;
        /*o segundo ponteiro do array de filhos aponta para o newnode que foi
        alterado na função validarInsercao*/
    }
}

int validarInsercao(arvore *raiz, int chave, int *chaveAadd, arvore **newnode)
{
    arvore *novoNo, *ultimoNo;
    /*caso ocorra o split precisamos criar um novo nó também criamos um ponteiro que vai apontar para o ultimo nó no array de filhos caso
    o array já estiver cheio no momento da inserção*/

    int pos, num_chaves, splitPos;
    /*inteiros para posição onde deve ser adicionado o nó, o num_chaves
    do nó e a posição que deve subir caso ocorra um split*/

    int novaChave, ultimaChave;
    /*uma nova chave que funcionará como a chaveAadd na recursividade e a ultimaChave que recebe a chave na ultima posição do array de chaves
    caso o array estiver cheio no momento da inserção*/

    int flag; //essa flag irá ser utilizada na recursividade
    if (raiz == NULL) // se não existir nada na árvore
    {
        *newnode = NULL; //não vai existir a necessidade de um novo nó
        *chaveAadd = chave;
        /*não vai ocorrer um split, portanto a chave que deve ser adicionada
        no nó "raiz" deve ser a própria chave passada no inserir*/

        return 1; //return a flag como 1
    }

    num_chaves = raiz->num_chaves; //essa variável existe apenas para facilitar a leitura do código

    pos = procurarPos(chave, raiz->chaves, num_chaves);
    //a função procurarPos vai retornar o filho onde a chave deve ser inserida

    if (pos < num_chaves && chave == raiz->chaves[pos]) return 0;
    /*caso a posição de inserçaõ for menor que o numero de chaves no array e a chave que se encontra
    atualmente na posição onde deveria ocorrer a inserção for igual a chave que se quer adicionar,
    a chave já existe na árvore!*/

    flag = validarInsercao(raiz->filhos[pos], chave, &novaChave, &novoNo);
    /*recursividade indo no filho do nó baseado no resultado da função procurarPos, a chave que se quer adicionar, a novaChave que funciona como a chaveAadd e o novoNo para caso ocorra o split*/

    if (flag != 1) return flag;
    /*se a flag for diferente de 1 significa que a inserção já
    aconteceu e não ocorreu o split*/

    if (num_chaves < ordem - 1)
    /*se o numero de chaves for menor que (ordem - 1) significa que
    o nó ainda não está cheio portanto podemos adicionar o valor
    nele sem problemas*/
    {
        pos = procurarPos(novaChave, raiz->chaves, num_chaves);
        /*o local onde deve ocorrer a inserção*/

        for (int i = num_chaves; i>pos; i--)
        /*vamos trazendo para trás no array as chaves que são
        maiores do que a chave que queremos adicionar*/
        {
            raiz->chaves[i] = raiz->chaves[i-1];
            //a chave move uma posição a direita no array;
            raiz->filhos[i+1] = raiz->filhos[i];
            //os ponteiros para filhos movem uma posição a direita
        }
        raiz->chaves[pos] = novaChave; //a posição recebe a novaChave
        raiz->filhos[pos+1] = novoNo;
        /*o ponteiro do array de filhos entre a novaChave e a chave
        imediatamente maior que ela vai apontar para o novoNo*/
        
        raiz->num_chaves++; //aumentando o contador de chaves no nó

        return 2;
        //se a flag for igual a 2 significa que a inserção já ocorreu
    }

    if (pos == ordem - 1)
    /*se a posição para se adicionar for a ultima e o nó
    já estiver cheio*/  
    {
        ultimaChave = novaChave;
        //ultimaChave vai guardar o valor da novaChave

        ultimoNo = novoNo;
        //ultimoNo vai guardar o valor do novoNo
    }
    else
    /*se a posição para se adicionar não for a ultima e o nó
    já estiver cheio*/  
    {
        ultimaChave = raiz->chaves[ordem-2];
        ultimoNo = raiz->filhos[ordem-1];
        for (int i=ordem-2; i>pos; i--)
        {
            raiz->chaves[i] = raiz->chaves[i-1];
            raiz->filhos[i+1] = raiz->filhos[i];
        }
        raiz->chaves[pos] = novaChave;
        raiz->filhos[pos+1] = novoNo;
    }
    splitPos = (ordem - 1)/2;
    (*chaveAadd) = raiz->chaves[splitPos];

    (*newnode)=malloc(sizeof(arvore));
    raiz->num_chaves = splitPos;
    (*newnode)->num_chaves = ordem-1-splitPos;
    for (int i=0; i < (*newnode)->num_chaves; i++)
    {
        (*newnode)->filhos[i] = raiz->filhos[i + splitPos + 1];
        if(i < (*newnode)->num_chaves - 1)
            (*newnode)->chaves[i] = raiz->chaves[i + splitPos + 1];
        else
            (*newnode)->chaves[i] = ultimaChave;
    }
    (*newnode)->filhos[(*newnode)->num_chaves] = ultimoNo;
    return 1;
}

void display(arvore *raiz, int espacos)
{
    if (raiz)
    {
        for(int i=1; i < espacos; i++)
            printf(" ");
        printf("[");
        for (int i = 0; i < raiz->num_chaves; i++){
          if(i != raiz->num_chaves-1) printf("%d ",raiz->chaves[i]);
          else printf("%d",raiz->chaves[i]);
        }
        printf("]");
        printf("\n");
        for (int i = 0; i <= raiz->num_chaves; i++)
            display(raiz->filhos[i], espacos+10);
    }
}

int procurarPos(int chave, int *chaves, int num_chaves)
  /*a função procurarPos vai receber uma chave, um array de chaves
  e o numero de chaves presentes no array*/
{
    int pos=0; //valor de iteração

    while (pos < num_chaves && chave > chaves[pos]) pos++;
    /*enquanto pos for menor que o numero de chaves no nó e o valor da chave for maior que o valor no array chaves na posição pos pos vai aumentar em 1, fazendo isso encontraremos posição onde a chave deve ser adicionada
    ou o filho para onde ela deve ser direcionada*/

    return pos;//retorna a posição
}
