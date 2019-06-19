#include <unistd.h>
#include <metadata.h>
#include <info_model.h>
#include <storage.h>
#include <leaf_node.h>

extern int ramificationFactor;
extern Metadata *meta;
extern InfoModel mainModel;

void setupStorage(char *catalogName, int degree)
{
    ramificationFactor = degree;
    if (access(METADATA_FILE_PATH, F_OK)) // If it exists, there's no need to setup the Store.
    {
        loadMetadata();
        return;
    }
    meta = initMetadata(degree);
    storeMetadata();

    // Initializing empty files
    FILE *internalFile, leafFile;
    internalFile = fopen(MAIN_INDEX_FILE_PATH, "wb");
    leafFile = fopen(DATA_FILE_PATH, "wb");
    fclose(internalFile);
    fclose(leafFile);

    Node *root = leafNodeCreate(degree);
    leafNodeStore(root);

    FILE *catalog = fopen(catalogName, "rb");
    if (!catalog)
        return;
    void *info;
    while (info = mainModel->infoLoader(catalog))
        insertOnTree(info);
    fclose(catalog);
}

void insertOnTree(void *info);

void *removeFromTree(int id);

void updateOnTree(int id, void *info);

void *getFromTree(int id);

void *getAllFromTree(void);

Node *loadRoot(void);

Node *loadNode(long pos);

void storeNode(Node *);
