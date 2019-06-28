#include <utils.h>
#include <pizza.h>

struct pizza
{
	int id;
	char name[50];
	char category[20];
	float price;
	int nextCategoryElement;
};

void pizzaPrint(void *v)
{
	Pizza *p = (Pizza *)v;
	printf("%d, %s (%s), R$ %.2f\n", p->id, p->name, p->category, p->price);
}

Pizza *pizzaCreate(int id, char *name, char *category, float price)
{
	Pizza *p = (Pizza *)malloc(sizeof(Pizza));
	if (p)
		memset(p, 0, sizeof(Pizza));
	p->id = id;
	strcpy(p->name, name);
	strcpy(p->category, category);
	p->price = price;
	p->nextCategoryElement = -1;
	return p;
}

void pizzaSave(void *v, FILE *out)
{
	Pizza *p = (Pizza *)v;
	fwrite(&p->id, sizeof(int), 1, out);
	fwrite(p->name, sizeof(char), sizeof(p->name), out);
	fwrite(p->category, sizeof(char), sizeof(p->category), out);
	fwrite(&p->price, sizeof(float), 1, out);
	fwrite(&p->nextCategoryElement, sizeof(int), 1, out);
}

void *pizzaRead(FILE *in)
{
	Pizza *p = (Pizza *)malloc(sizeof(Pizza));
	if (0 >= fread(&p->id, sizeof(int), 1, in))
	{
		free(p);
		return NULL;
	}
	fread(p->name, sizeof(char), sizeof(p->name), in);
	fread(p->category, sizeof(char), sizeof(p->category), in);
	fread(&p->price, sizeof(float), 1, in);
	fread(&p->nextCategoryElement, sizeof(int), 1, in);
	return (void *)p;
}

int pizzaCmp(Pizza *p1, Pizza *p2)
{
	if (p1 == NULL || p2 == NULL)
	{
		return (p2 == NULL) && (p1 == NULL);
	}
	if (p1->id != p2->id)
	{
		return 0;
	}
	if (strcmp(p1->name, p2->name) != 0)
	{
		return 0;
	}
	if (strcmp(p1->category, p2->category) != 0)
	{
		return 0;
	}
	if (p1->price != p2->price)
	{
		return 0;
	}
	return 1;
}

int pizzaSize(void)
{
	return sizeof(int) +	   // id
		   sizeof(char) * 50 + // name
		   sizeof(char) * 20 + // categoria
		   sizeof(float) +	 // preço
		   sizeof(int);		   // proximoElementoCategoria
}

void pizzaFree(void *p)
{
	free(p);
}

int pizzaGetId(void *v)
{
	Pizza *p = (Pizza *)v;
	return p->id;
}

void pizzaSetId(void *v, int id)
{
	Pizza *p = (Pizza *)v;
	p->id = id;
}

void *pizzaCategory(void *v)
{
	Pizza *p = (Pizza *)v;
	return (void *)(p->category);
}

int getNextCategoryPosition(void *v)
{
	Pizza *p = (Pizza *)v;
	return p->nextCategoryElement;
}

void setNextCategoryPosition(void *v, int position)
{
	Pizza *p = (Pizza *)v;
	p->nextCategoryElement = position;
}

char *pizzaName(bool plural)
{
	if (plural)
		return "pizzas";
	else
		return "pizza";
}
