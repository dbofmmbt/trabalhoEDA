#include <unistd.h>
#include <metadata.h>
#include <storage.h>
#include <leaf_node.h>

extern int ramificationFactor;
extern Metadata *meta;

void setupStore(char *pizzasFile, int degree)
{
    if (access(METADATA_FILE_PATH, F_OK)) // If it exists, there's no need to setup the Store.
        return;
    ramificationFactor = degree;
    meta = initMetadata(degree);
    storeMetadata();

    // Iniciando os arquivos vazios
    FILE *internalFile, leafFile;
    internalFile = fopen(MAIN_INDEX_FILE_PATH, "wb");
    leafFile = fopen(DATA_FILE_PATH, "wb");
    fclose(internalFile);
    fclose(leafFile);

    Node *root = leafNodeCreate(degree);
    /* TODO */
}

void insertOnTree(void *info);

void *removeFromTree(int id);

void updateOnTree(int id, void *info);

void *getFromTree(int id);

void *getAllFromTree(void);

Node *loadRoot(void);

Node *loadNode(long pos);

void storeNode(Node *);
