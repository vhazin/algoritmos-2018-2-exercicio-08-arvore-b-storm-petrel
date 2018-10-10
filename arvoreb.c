#include <stdio.h>
#include <stdlib.h>
#define ordem 5

struct node {
    int n;
    int chaves[ordem-1];
    struct node *filhos[ordem];
}*root = NULL;

enum KeyStatus { Duplicado,Falhou,Sucesso,Insira,MenosChaves };

void insert(int chave);
void display(struct node *root,int);
enum KeyStatus ins(struct node *r, int x, int* y, struct node** u);
int searchPos(int x,int *key_arr, int n);
void eatline(void);

int main()
{
    int chave;
    int choice;
    printf("Criação de uma árvore B de Ordem=%d\n",ordem);
    while(1)
    {
        printf("1.Inserir\n");
        printf("2.Mostrar\n");
        printf("3.Sair\n");
        printf("Digite a sua escolha : ");
        scanf("%d",&choice); eatline();

        switch(choice)
        {
        case 1:
            printf("Digite a chave : ");
            scanf("%d",&chave); eatline();
            insert(chave);
            break;
        case 2:
            printf("árvore B:\n");
            display(root,0);
            break;
        case 3:
            exit(1);
        default:
            printf("Wrong choice\n");
            break;
        }/*End of switch*/
    }/*End of while*/
    return 0;
}

void insert(int chave)
{
    struct node *newnode;
    int upKey;
    enum KeyStatus value;
    value = ins(root, chave, &upKey, &newnode);
    if (value == Duplicado)
        printf("Chave já existe\n");
    if (value == Insira)
    {
        struct node *uproot = root;
        root=malloc(sizeof(struct node));
        root->n = 1;
        root->chaves[0] = upKey;
        root->filhos[0] = uproot;
        root->filhos[1] = newnode;
    }
}

enum KeyStatus ins(struct node *ptr, int chave, int *upKey,struct node **newnode)
{
    struct node *newPtr, *lastPtr;
    int pos, i, n,splitPos;
    int newKey, lastKey;
    enum KeyStatus value;
    if (ptr == NULL)
    {
        *newnode = NULL;
        *upKey = chave;
        return Insira;
    }
    n = ptr->n;
    pos = searchPos(chave, ptr->chaves, n);
    if (pos < n && chave == ptr->chaves[pos])
        return Duplicado;
    value = ins(ptr->filhos[pos], chave, &newKey, &newPtr);
    if (value != Insira)
        return value;
    /*If chaves in node is less than M-1 where M is order of B tree*/
    if (n < ordem - 1)
    {
        pos = searchPos(newKey, ptr->chaves, n);
        /*Shifting the chave and pointer right for inserting the new chave*/
        for (i=n; i>pos; i--)
        {
            ptr->chaves[i] = ptr->chaves[i-1];
            ptr->filhos[i+1] = ptr->filhos[i];
        }
        /*Key is inserted at exact location*/
        ptr->chaves[pos] = newKey;
        ptr->filhos[pos+1] = newPtr;
        ++ptr->n; /*incrementing the number of chaves in node*/
        return Sucesso;
    }/*End of if */
    /*If chaves in nodes are maximum and position of node to be inserted is last*/
    if (pos == ordem - 1)
    {
        lastKey = newKey;
        lastPtr = newPtr;
    }
    else /*If chaves in node are maximum and position of node to be inserted is not last*/
    {
        lastKey = ptr->chaves[ordem-2];
        lastPtr = ptr->filhos[ordem-1];
        for (i=ordem-2; i>pos; i--)
        {
            ptr->chaves[i] = ptr->chaves[i-1];
            ptr->filhos[i+1] = ptr->filhos[i];
        }
        ptr->chaves[pos] = newKey;
        ptr->filhos[pos+1] = newPtr;
    }
    splitPos = (ordem - 1)/2;
    (*upKey) = ptr->chaves[splitPos];

    (*newnode)=malloc(sizeof(struct node));/*Right node after split*/
    ptr->n = splitPos; /*No. of chaves for left splitted node*/
    (*newnode)->n = ordem-1-splitPos;/*No. of chaves for right splitted node*/
    for (i=0; i < (*newnode)->n; i++)
    {
        (*newnode)->filhos[i] = ptr->filhos[i + splitPos + 1];
        if(i < (*newnode)->n - 1)
            (*newnode)->chaves[i] = ptr->chaves[i + splitPos + 1];
        else
            (*newnode)->chaves[i] = lastKey;
    }
    (*newnode)->filhos[(*newnode)->n] = lastPtr;
    return Insira;
}/*End of ins()*/

void display(struct node *ptr, int blanks)
{
    if (ptr)
    {
        int i;
        for(i=1; i<=blanks; i++)
            printf(" ");
        for (i=0; i < ptr->n; i++)
            printf("%d ",ptr->chaves[i]);
        printf("\n");
        for (i=0; i <= ptr->n; i++)
            display(ptr->filhos[i], blanks+10);
    }/*End of if*/
}/*End of display()*/

int searchPos(int chave, int *key_arr, int n)
{
    int pos=0;
    while (pos < n && chave > key_arr[pos])
        pos++;
    return pos;
}/*End of searchPos()*/

void eatline(void) {
  char c;
  printf("");
  while (c=getchar()!='\n') ;
}
