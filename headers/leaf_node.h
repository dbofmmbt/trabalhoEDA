#include <node.h>

typedef struct leafNode LeafNode;

Node *leafNodeCreate(int t);

/* if pos = -1, saves on the end of file. Returns the position where the node is saved. */
Address leafNodeStore(void *node, Address pos);

Node *leafNodeLoad(Address pos); // TODO
