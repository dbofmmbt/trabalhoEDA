#ifndef STORAGE
#define STORAGE

#include <utils.h>
#include <metadata.h>
#include <info_model.h>
#include <leaf_node.h>
#include <internal_node.h>
#include <rotations.h>


void setupStorage(char *catalogName, int degree);

void insertOnTree(void *info);

void *removeFromTree(int id);

bool updateOnTree(void *info);

void *getFromTree(int id);

void *forEachInfo(void (*callback)(void *));

void *printAllFromSecIndex(void (*callback)(void *), void *secIndex);

void *removeAllFromSecIndex(void *secIndex);

void printTree(void);

#endif
