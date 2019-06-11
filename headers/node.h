typedef struct node
{
    void *(*nodeSearch)(void *this, int id);
    struct node *(*nodeInsert)(void *this, void *info);
    struct node *(*nodeRemove)(void *this, int id);
} Node;