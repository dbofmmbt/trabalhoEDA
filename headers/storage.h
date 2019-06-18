#include <utils.h>
#include <info_model.h>
#include <node.h>

#define SEC_INDEX_FILE_PATH "data/sec_index.bin"
#define MAIN_INDEX_FILE_PATH "data/main_index.bin"
#define DATA_FILE_PATH "data/data.bin"
#define METADATA_FILE_PATH "data/metadata.bin"

int ramificationFactor; // ? Does it make sense to keep it on metadata?

void setupStorage(char *catalogName, int degree);

void insertOnTree(void *info); // TODO

void *removeFromTree(int id); // TODO

bool updateOnTree(void *info); // TODO

void *getFromTree(int id); // TODO

void *getAllFromTree(void); // TODO
