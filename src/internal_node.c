#include <internal_node.h>

struct internalNode
{
    Node;
    int vet[30];
    bool isPointingToLeaf;
};

Node *internalNodeCreate(int t)
{
}
static void *_internalNodeSearch(void *this, int id);
static Node *_internalNodeInsert(void *this, void *info);
static Node *_internalNodeRemove(void *this, int id);