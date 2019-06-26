#ifndef STORAGE
#define STORAGE

#include <utils.h>
#include <storage/metadata.h>
#include <interfaces/info_model.h>
#include <storage/leaf_node.h>
#include <storage/internal_node.h>
#include <storage/rotations.h>

int ramificationFactor; // ? Does it make sense to keep it on metadata?

void setupStorage(char *catalogName, int degree);

void insertOnTree(void *info); // TODO

void *removeFromTree(int id); // TODO

bool updateOnTree(void *info); // TODO

void *getFromTree(int id); // TODO

void *getAllFromTree(void); // TODO

#endif
