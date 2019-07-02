#ifndef PIZZA
#define PIZZA

#include <utils.h>

typedef struct pizza
{
	int id;
	char name[50];
	char category[20];
	float price;
} Pizza;

/* When nextCategoryElement is -1, it means that there's no next element */

// Imprime pizza
void pizzaPrint(void *p);

// Cria pizza
Pizza *pizzaCreate(int id, char *name, char *category, float price);

// Salva pizza no arquivo out, na posicao atual do cursor
void pizzaSave(void *p, FILE *out);

// Le uma pizza do arquivo in na posicao atual do cursor
// Retorna um ponteiro para pizza lida do arquivo
void *pizzaRead(FILE *in);

// Compara duas pizzas
// Retorna 1 se os valores dos atributos de ambos forem iguais
// e 0 caso contrario
int pizzaCmp(Pizza *p1, Pizza *p2);

// Retorna tamanho do registro que representa a pizza em bytes
int pizzaSize(void);

void pizzaFree(void *p);

int pizzaGetId(void *p);

void pizzaSetId(void *p, int id);

void *pizzaCategory(void *p);

char *pizzaName(bool plural);

#endif