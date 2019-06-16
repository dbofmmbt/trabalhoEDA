#include <utils.h>

typedef struct node
{
    void *(*nodeSearch)(void *this, int id);

    struct node *(*nodeInsert)(void *this, void *info);

    struct node *(*nodeRemove)(void *this, int id);

    struct node *(*nodeGet)(void *this, int i);

    int (*nodeLenght)(void *this);

    int (*nodeSizeInBytes)(void *this);

} Node;