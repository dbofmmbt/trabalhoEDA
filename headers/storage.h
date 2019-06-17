#include <utils.h>
#include <info_model.h>
#include <node.h>

#define SEC_INDEX_FILE_PATH "data/sec_index.bin"
#define MAIN_INDEX_FILE_PATH "data/main_index.bin"
#define METADATA_FILE_PATH "data/metadata.bin"

int ramificationFactor;

void setupStore(char *pizzasFile, int degree);

Node *loadRoot(void);

Node *loadNode(long pos);

void storeNode(Node *);

saveData(void *info);