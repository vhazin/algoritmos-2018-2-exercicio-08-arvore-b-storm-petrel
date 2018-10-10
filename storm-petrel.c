//João Victor Bravo, Matheus Farias, Tiago Valença

#include <stdio.h>
#include <stdlib.h>
#define ordem 5

struct node {
    int num_chaves;
    int chaves[ordem-1];
    struct node *filhos[ordem];
    int h;
}*root = NULL;

void insert(int chave);
void display(struct node *root,int);
int ins(struct node *r, int x, int* y, struct node** u);
int searchPos(int x,int *key_arr, int num_chaves);
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
        }
    }
    return 0;
}

void insert(int chave)
{
    struct node *newnode;
    int upKey;
    int value;
    value = ins(root, chave, &upKey, &newnode);
    if (value == 0)
        printf("Chave já existe\n");
    if (value == 1)
    {
        struct node *uproot = root;
        root=malloc(sizeof(struct node));
        root->num_chaves = 1;
        root->chaves[0] = upKey;
        root->filhos[0] = uproot;
        root->filhos[1] = newnode;
    }
}

int ins(struct node *ptr, int chave, int *upKey,struct node **newnode)
{
    struct node *newPtr, *lastPtr;
    int pos, i, num_chaves,splitPos;
    int newKey, lastKey;
    int value;
    if (ptr == NULL)
    {
        *newnode = NULL;
        *upKey = chave;
        return 1;
    }
    num_chaves = ptr->num_chaves;
    pos = searchPos(chave, ptr->chaves, num_chaves);
    if (pos < num_chaves && chave == ptr->chaves[pos])
        return 0;
    value = ins(ptr->filhos[pos], chave, &newKey, &newPtr);
    if (value != 1)
        return value;
    if (num_chaves < ordem - 1)
    {
        pos = searchPos(newKey, ptr->chaves, num_chaves);
        for (i=num_chaves; i>pos; i--)
        {
            ptr->chaves[i] = ptr->chaves[i-1];
            ptr->filhos[i+1] = ptr->filhos[i];
        }
        ptr->chaves[pos] = newKey;
        ptr->filhos[pos+1] = newPtr;
        ++ptr->num_chaves;
        return 3;
    }
    if (pos == ordem - 1)
    {
        lastKey = newKey;
        lastPtr = newPtr;
    }
    else
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

    (*newnode)=malloc(sizeof(struct node));
    ptr->num_chaves = splitPos;
    (*newnode)->num_chaves = ordem-1-splitPos;
    for (i=0; i < (*newnode)->num_chaves; i++)
    {
        (*newnode)->filhos[i] = ptr->filhos[i + splitPos + 1];
        if(i < (*newnode)->num_chaves - 1)
            (*newnode)->chaves[i] = ptr->chaves[i + splitPos + 1];
        else
            (*newnode)->chaves[i] = lastKey;
    }
    (*newnode)->filhos[(*newnode)->num_chaves] = lastPtr;
    return 1;
}

void display(struct node *ptr, int blanks)
{
    if (ptr)
    {
        int i;
        for(i=1; i<=blanks; i++)
            printf(" ");
        printf("[");
        for (i=0; i < ptr->num_chaves; i++){
          if(i!=ptr->num_chaves-1) printf("%d ",ptr->chaves[i]);
          else printf("%d",ptr->chaves[i]);
        }
        printf("]");
        printf("\n");
        for (i=0; i <= ptr->num_chaves; i++)
            display(ptr->filhos[i], blanks+10);
    }
}

int searchPos(int chave, int *key_arr, int num_chaves)
{
    int pos=0;
    while (pos < num_chaves && chave > key_arr[pos])
        pos++;
    return pos;
}

void eatline(void) {
  char c;
  printf("");
  while (c=getchar()!='\n') ;
}
