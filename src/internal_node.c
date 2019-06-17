#include <internal_node.h>

struct internalNode
{
    Node;
    int *IDs;
    bool isPointingToLeaf;
};

Node *internalNodeCreate(int t)
{
    extern int ramificationFactor;
    int keysNumber = (ramificationFactor * 2) - 1;
    InternalNode *new_node = (InternalNode *)malloc(sizeof(InternalNode));
    new_node->isPointingToLeaf = false;
    new_node->IDs = (int *)malloc(sizeof(int) * keysNumber);
}
static void *_internalNodeSearch(void *this, int id);
static Node *_internalNodeInsert(void *this, void *info);
static Node *_internalNodeRemove(void *this, int id);