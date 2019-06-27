#ifndef STORAGE
#define STORAGE

#include <utils.h>
#include <metadata.h>
#include <info_model.h>
#include <leaf_node.h>
#include <internal_node.h>

int ramificationFactor; // ? Does it make sense to keep it on metadata?

void setupStorage(char *catalogName, int degree);

void insertOnTree(void *info); // TODO

void *removeFromTree(int id); // TODO

bool updateOnTree(void *info); // TODO

void *getFromTree(int id); // TODO

void *getAllFromTree(void); // TODO

#endif