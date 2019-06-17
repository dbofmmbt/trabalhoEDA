#include <storage.h>
#include <unistd.h>

void setupStore(char *pizzasFile, int degree)
{
    extern int ramificationFactor;
    if (access(METADATA_FILE_PATH, F_OK)) // If it exists, there's no need to setup the Store.
        return;
    ramificationFactor = degree;
    
}

Node *loadRoot(void);

Node *loadNode(long pos);

void storeNode(Node *);

saveData(void *info);