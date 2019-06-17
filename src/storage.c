#include <storage.h>
#include <unistd.h>

void setupStore(char *pizzasFile, int degree)
{
    if (access(METADATA_FILE_PATH, F_OK)) // If it exists, there's no need to setup the Store.
        return;
    extern int ramificationDegree = degree;
    
}

Node *loadRoot(void);

Node *loadNode(long pos);

void storeNode(Node *);

saveData(void *info);