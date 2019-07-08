#ifndef MOVIE
#define MOVIE

#include <utils.h>

typedef struct movie
{
	int id;
	char title[100];
	char gender[20];
    char director[50];
    char nationality[50];
    int launch_year;
    int duration;
} Movie;

/* When nextCategoryElement is -1, it means that there's no next element */

// Imprime o filme
void moviePrint(void *p);

// Cria o filme
Movie *movieCreate(int id, char *title, char *gender,char * director,char * nationality, int launch_year, int duration);

//pega os dados de um filme a partir de input do usuario
void *getMovieFromUser(void);

//cria um filme de exemplo
void *getSampleMovie(void);

// Salva movie no arquivo out, na posicao atual do cursor
void movieSave(void *m, FILE *out);

// Le uma movie do arquivo in na posicao atual do cursor
// Retorna um ponteiro para filme lido do arquivo
void *movieRead(FILE *in);

// Compara dois filmes
// Retorna 1 se os valores dos atributos de ambos forem iguais
// e 0 caso contrario
int movieCmp(Movie *m1, Movie *m2);

// Retorna tamanho do registro que representa o filme em bytes
int movieSize(void);

void movieFree(void *m);

int movieGetId(void *m);

void movieSetId(void *m, int id);

void *movieGender(void *m);

char *movieName(bool plural);

char *movieSecIndexName(bool plural);

#endif
