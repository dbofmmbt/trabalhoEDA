#ifndef STORAGE
#define STORAGE

#include <utils.h>
#include <metadata.h>
#include <info_model.h>
#include <leaf_node.h>
#include <internal_node.h>
#include <rotations.h>

int branchingFactor; // ? Does it make sense to keep it on metadata?

void setupStorage(char *catalogName, int degree);

void insertOnTree(void *info); // TODO

void *removeFromTree(int id); // TODO

bool updateOnTree(void *info);

void *getFromTree(int id);

void *forEachInfo(void (*callback)(void *));

void *printAllFromSecIndex(void (*callback)(void *), void *secIndex);

void *removeAllFromSecIndex(void (*callback)(void *), void *secIndex);

#endif
