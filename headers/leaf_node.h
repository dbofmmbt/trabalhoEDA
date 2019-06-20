#include <node.h>

typedef struct leafNode LeafNode;

Node *leafNodeCreate(int t);

/* if pos = -1, saves on the end of file. Returns the position where the node is saved. */
int leafNodeStore(void *node, int pos);
