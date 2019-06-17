#include <store.h>

void setupStore(char *pizzasFile, int degree);

Node *loadRoot(void);

Node *loadNode(long pos);

void storeNode(Node *);

saveData(void *info);