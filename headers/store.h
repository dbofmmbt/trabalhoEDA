#include <utils.h>
#include <info_model.h>
#include <node.h>

#define SEC_INDEX_FILE_NAME "sec_index.bin"
#define MAIN_INDEX_FILE_NAME "main_index.bin"
#define METADATA_FILE_NAME "metadata.bin"

void setupStore(char *pizzasFile, int treeDegree);

Node *loadRoot(void);

Node *loadNode(long pos);

void storeNode(Node *);

saveData(void *info);