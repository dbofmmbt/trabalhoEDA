#include <stdio.h>
#include <stdlib.h>

struct no
{
    int (*get_id)(void *self, int mat);
};

struct noFolha
{
    struct no;
    struct info *dados;
};

struct noInterno
{
    struct no;
    int *vet;
    struct no **filho;
};

struct info
{
    int mat;
    int id;
};

int noFolha_get_id(void *self, int pos)
{
    struct noFolha *this = (struct noFolha *)self;
    return this->dados->id;
}

int noInterno_get_id(void *self, int pos)
{
    struct noInterno *this = (struct noInterno *)self;
    return this->vet[0];
}

struct noFolha *cria_folha()
{
    struct noFolha *novo = (struct noFolha *)malloc(sizeof(struct noFolha));
    novo->dados = (struct info *)malloc(sizeof(struct info));
    novo->dados->id = 42;
    novo->dados->mat = 1;
    novo->get_id = noFolha_get_id;
    return novo;
}

struct noInterno *cria_interno()
{
    struct noInterno *novo = (struct noInterno *)malloc(sizeof(struct noInterno));
    novo->vet = (int *)malloc(sizeof(int));
    novo->vet[0] = 64;
    novo->get_id = noInterno_get_id;
    return novo;
}

int main(int argc, char const *argv[])
{
    struct no *n = (struct no *)cria_folha();
    printf("%d\n", n->get_id(n, 0));
    n = (struct no *)cria_interno();
    printf("%d\n", n->get_id(n, 0));

    return 0;
}
