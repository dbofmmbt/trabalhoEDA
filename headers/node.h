#include <utils.h>

typedef struct node
{
    int numberKeys;

    void *(*nodeSearch)(void *this, int id);

    struct node *(*nodeInsert)(void *this, void *info);

    struct node *(*nodeRemove)(void *this, int id);

    struct node *(*nodeGet)(void *this, int i);

    int (*nodeLenght)(void *this);

    int (*nodeSizeInBytes)(void *this);

} Node;