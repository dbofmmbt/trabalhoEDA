#include <storage.h>
#include <unistd.h>

void setupStore(char *pizzasFile, int degree)
{
    extern int ramificationFactor;
    ramificationFactor = degree;
    if (access(METADATA_FILE_PATH, F_OK)) // If it exists, there's no need to setup the Store.
        return;
    
}

void insertOnTree(void *info);

void *removeFromTree(int id);

void updateOnTree(int id, void *info);

void *getFromTree(int id);

void *getAllFromTree(void);

Node *loadRoot(void);

Node *loadNode(long pos);

void storeNode(Node *);
