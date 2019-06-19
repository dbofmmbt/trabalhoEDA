#include <utils.h>
#include <info_model.h>
#include <node.h>

#define SEC_INDEX_FILE_PATH "data/sec_index.bin"
#define MAIN_INDEX_FILE_PATH "data/main_index.bin"
#define DATA_FILE_PATH "data/data.bin"
#define METADATA_FILE_PATH "data/metadata.bin"

int ramificationFactor;

void setupStore(char *pizzasFile, int degree);

void insertOnTree(void *info);

void *removeFromTree(int id);

bool updateOnTree(void *info);

void *getFromTree(int id);

void *getAllFromTree(void);