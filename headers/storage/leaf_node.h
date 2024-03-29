#ifndef LEAF_NODE
#define LEAF_NODE

#include <utils.h>

typedef struct leafNode
{
    int numberOfKeys;
    void **info;
    Address next;
} LeafNode;

LeafNode *leafNodeCreate();

/* if pos = -1, saves on the end of file. Returns the position where the node is saved. */
Address leafNodeStore(LeafNode *node, Address pos);

LeafNode *leafNodeLoad(Address pos);

void leafNodeFree(LeafNode *node);

Address leafNodeInfoAddress(Address nodeAddress, int index);

#endif
