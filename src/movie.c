#include <utils.h>
#include <movie.h>

void moviePrint(void *v)
{
	Movie *m = (Movie *)v;
	printf("%d, %s - %i - %s -(%s), %d minutes - diretor: %s \n", m->id, m->title, m->launch_year, m->nationality, m->gender, m->duration, m->director);
}

Movie *movieCreate(int id, char *title, char *gender, char * director,char * nationality, int launch_year, int duration)
{
	Movie *m = (Movie *)malloc(sizeof(Movie));
	if (!m)
		return m;
	memset(m, 0, sizeof(Movie));
	m->id = id;
	strcpy(m->title, title);
	strcpy(m->gender, gender);
    strcpy(m->director, director);
    strcpy(m->nationality, nationality);
    m->launch_year = launch_year;
	m->duration = duration;
	return m;
}

void *getMovieFromUser(void)
{
	char movieTitle[100];
	char movieGender[20];
    char movieDirector[50];
    char movieNationality[50];
	int movieLaunchYear;
    int movieDuration;

	printf("Titulo: ");
	scanf(" %[^\n]", movieTitle);
	printf("Gênero: ");
	scanf(" %[^\n]", movieGender);
    printf("Diretor: ");
    scanf(" %[^\n]", movieDirector);
    printf("Nacionalidade: ");
    scanf(" %[^\n]", movieNationality);
    printf("Ano de Lançamento: ");
    scanf("%d", &movieLaunchYear);
    printf("Duração do filme (em minutos): ");
    scanf("%d", &movieDuration);

	Movie *m = movieCreate(0, movieTitle, movieGender, movieDirector, movieNationality, movieLaunchYear, movieDuration);
	return (void *)m;
}

void *getSampleMovie(void)
{
	return (void *)movieCreate(0, "Titulo", "Genero", "Diretor", "Nacionalidade", 2010, 160);
}

void movieSave(void *p, FILE *out)
{
	if (!p)
		return;

	Movie *m = (Movie *)p;
	fwrite(&m->id, sizeof(int), 1, out);
	fwrite(m->title, sizeof(char), sizeof(m->title), out);
	fwrite(m->gender, sizeof(char), sizeof(m->gender), out);
    fwrite(m->director, sizeof(char), sizeof(m->director), out);
    fwrite(m->nationality, sizeof(char), sizeof(m->nationality), out);
    fwrite(&m->launch_year, sizeof(int), 1, out);
	fwrite(&m->duration, sizeof(int), 1, out);
}

void *movieRead(FILE *in)
{
	Movie *m = (Movie *)malloc(sizeof(Movie));
	if (!m)
		return m;
	if (!fread(&m->id, sizeof(int), 1, in))
	{
		free(m);
		return NULL;
	}

	fread(m->title, sizeof(char), sizeof(m->title), in);
	fread(m->gender, sizeof(char), sizeof(m->gender), in);
	fread(m->director, sizeof(char), sizeof(m->director), in);
	fread(m->nationality, sizeof(char), sizeof(m->nationality), in);
	fread(&m->launch_year, sizeof(int), 1, in);
	fread(&m->duration, sizeof(int), 1, in);


	return (void *)m;
}

int movieCmp(Movie *m1, Movie *m2){
	if (m1 == NULL || m2 == NULL)
	{
		return (m2 == NULL) && (m1 == NULL);
	}
	if (m1->id != m2->id)
	{
		return 0;
	}
	if (strcmp(m1->title, m2->title) != 0)
	{
		return 0;
	}
	if (strcmp(m1->gender, m2->gender) != 0)
	{
		return 0;
	}
	if (strcmp(m1->director, m2->director) != 0)
	{
		return 0;
	}
	if (strcmp(m1->nationality, m2->nationality) != 0)
	{
		return 0;
	}
	if (m1->launch_year != m2->launch_year)
	{
		return 0;
	}
	if (m1->duration != m2->duration)
	{
		return 0;
	}
	return 1;
}

int movieSize(void)
{
	return sizeof(int) +	   // id
		   sizeof(char) * 100 + // title
		   sizeof(char) * 20 + // gender
		   sizeof(char) * 50 + //director
		   sizeof(char) * 50 + //nationality
		   sizeof(int) + //launch year
		   sizeof(int);	  // duration
}

void movieFree(void *m)
{
	if (m)
		free(m);
}

int movieGetId(void *p)
{
	Movie *m = (Movie *)p;
	return m->id;
}

void movieSetId(void *p, int id)
{
	Movie *m = (Movie *)p;
	m->id = id;
}

void *movieGender(void *p)
{
	Movie *m = (Movie *)p;
	return (void *)(m->gender);
}

char *movieName(bool plural)
{
	if (plural)
		return "Filmes";
	else
		return "Filme";
}

char *movieSecIndexName(bool plural)
{
	if (plural)
		return "Gêneros";
	else
		return "Gênero";
}
