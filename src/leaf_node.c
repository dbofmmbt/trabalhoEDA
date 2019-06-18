#include <leaf_node.h>

struct leafNode
{
    Node;
    void *info;
    struct leafNode *nextLeafNode;
};

Node *leafNodeCreate(int t);
static void *_leafNodeSearch(void *this, int id);
static Node *_leafNodeInsert(void *this, void *info);
static Node *_leafNodeRemove(void *this, int id);