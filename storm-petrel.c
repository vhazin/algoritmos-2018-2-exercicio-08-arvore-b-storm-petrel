//João Victor Bravo, Matheus Farias, Tiago Valença
#include <stdio.h>
#include <stdlib.h>
#define ordem 5 //a ordem da árvore só pode ser alterada antes do programa ser iniciado

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
int searchPos(int x,int *key_arr, int num_chaves);

int main()
{
    int chave;//chave que irá ser adicionada
    int escolha;//variavel para escolher dentre as funcionalidades do programa
    printf("Criação de uma árvore B de Ordem=%d\n",ordem);
    while(1)
    {
        printf("\n");
        printf("1.Inserir\n");
        printf("2.Mostrar a árvore\n");
        printf("3.Sair\n");
        printf("Digite a sua escolha : ");
        scanf("%d",&escolha);

        switch(escolha)//switch que recebe a variavel escolha
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
        default:
            printf("Faça uma escolha válida\n");
            break; //break para sair do switch e voltar para o menu
        }
    }
    return 0;
}

void inserir(int chave)//a função inserir recebe um valor inteiro "chave"
{
    arvore *newnode; //criação de um novo nó que poderá ser utilizado caso o numero de valores no nó raiz após a insercao seja maior que ordem-1
    int chaveAadd; //chave que será adicionada ao final da função inserir
    int flag; //uma Flag que indica se o valor da chave já existe na árvore ou não
    flag = validarInsercao(root, chave, &chaveAadd, &newnode); //a flag vai receber o valor da funçao validarInsercao
    if (!flag) //se a flag for falsa a chave já existe na árvore
        printf("Chave já existe\n");
    if (flag == 1) //se a flag for 1 significa que ocorreu o split no nó anterior então deveremos criar um novo nó pai
    {
        arvore *filhoE = root; //criamos um nó filhoE que vai ser igual ao no raiz que foi alterado na função validarInsercao
        root = malloc(sizeof(arvore));
        root->num_chaves = 1; //o nó pai vai ter 1 valor que subiu no split
        root->chaves[0] = chaveAadd; //o valor que vai subir no split é a chaveAadd
        root->filhos[0] = filhoE; //o primeiro ponteiro do array de filhos aponta para o filhoE
        root->filhos[1] = newnode; //o segundo ponteiro do array de filhos aponta para o newnode que foi alterado na função validarInsercao
    }
}

int validarInsercao(arvore *raiz, int chave, int *chaveAadd, arvore **newnode)
{
    arvore *novoNo, *ultimoNo;
    int pos, i, num_chaves, splitPos;
    int novaChave, ultimaChave;
    int flag;
    if (raiz == NULL)
    {
        *newnode = NULL;
        *chaveAadd = chave;
        return 1;
    }
    num_chaves = raiz->num_chaves;
    pos = searchPos(chave, raiz->chaves, num_chaves);
    if (pos < num_chaves && chave == raiz->chaves[pos])
        return 0;
    flag = validarInsercao(raiz->filhos[pos], chave, &novaChave, &novoNo);
    if (flag != 1)
        return flag;
    if (num_chaves < ordem - 1)
    {
        pos = searchPos(novaChave, raiz->chaves, num_chaves);
        for (i=num_chaves; i>pos; i--)
        {
            raiz->chaves[i] = raiz->chaves[i-1];
            raiz->filhos[i+1] = raiz->filhos[i];
        }
        raiz->chaves[pos] = novaChave;
        raiz->filhos[pos+1] = novoNo;
        raiz->num_chaves++;
        return 3;
    }
    if (pos == ordem - 1)
    {
        ultimaChave = novaChave;
        ultimoNo = novoNo;
    }
    else
    {
        ultimaChave = raiz->chaves[ordem-2];
        ultimoNo = raiz->filhos[ordem-1];
        for (i=ordem-2; i>pos; i--)
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
    for (i=0; i < (*newnode)->num_chaves; i++)
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
        int i;
        for(i=1; i < espacos; i++)
            printf(" ");
        printf("[");
        for (i = 0; i < raiz->num_chaves; i++){
          if(i != raiz->num_chaves-1) printf("%d ",raiz->chaves[i]);
          else printf("%d",raiz->chaves[i]);
        }
        printf("]");
        printf("\n");
        for (i = 0; i <= raiz->num_chaves; i++)
            display(raiz->filhos[i], espacos+10);
    }
}

int searchPos(int chave, int *key_arr, int num_chaves)
{
    int pos=0;
    while (pos < num_chaves && chave > key_arr[pos])
        pos++;
    return pos;
}
