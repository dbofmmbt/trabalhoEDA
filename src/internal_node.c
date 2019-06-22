#include <internal_node.h>

struct internalNode
{
    int numberKeys;
    int vet[30];
    bool isPointingToLeaf;
};

InternalNode *internalNodeCreate(int t);
