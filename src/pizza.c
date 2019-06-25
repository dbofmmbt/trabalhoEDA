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

void pizzaPrint(Pizza *p)
{
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

void pizzaSave(Pizza *p, FILE *out)
{
	fwrite(&p->id, sizeof(int), 1, out);
	fwrite(p->name, sizeof(char), sizeof(p->name), out);
	fwrite(p->category, sizeof(char), sizeof(p->category), out);
	fwrite(&p->price, sizeof(float), 1, out);
	fwrite(&p->nextCategoryElement, sizeof(int), 1, out);
}

Pizza *pizzaRead(FILE *in)
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
	return p;
}

int pizzaCmp(Pizza *p1, Pizza *p2)
{
	if (p1 == NULL)
	{
		return (p2 == NULL);
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

void pizzaFree(Pizza *p)
{
	free(p);
}

int pizzaGetId(Pizza *p)
{
	return p->id;
}

void setId(Pizza *p, int id)
{
	p->id = id;
}

char *pizzaCategory(Pizza *p)
{
	return p->category;
}

int getNextCategoryPosition(Pizza *p)
{
	return p->nextCategoryElement;
}

void setNextCategoryPosition(Pizza *p, int position)
{
	p->nextCategoryElement = position;
}

char *pizzaName(bool plural)
{
	if (plural)
		return "pizzas";
	else
		return "pizza";
}
