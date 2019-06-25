#include <utils.h>

typedef struct leafNode LeafNode;

LeafNode *leafNodeCreate(int t);

/* if pos = -1, saves on the end of file. Returns the position where the node is saved. */
Address leafNodeStore(LeafNode *node, Address pos);

LeafNode *leafNodeLoad(Address pos);

void LeafNodeFree(LeafNode *node);

void leafNodeDivision(Address father, int sonPosition);
