#include <internal_node.h>
extern InfoModel mainModel;

struct internalNode
{
    Node;
    int *IDs;
    bool isPointingToLeaf;
    int *children;
};

Node *internalNodeCreate()
{
    extern int ramificationFactor;
    int keysNumber = (ramificationFactor * 2) - 1;
    InternalNode *newNode = (InternalNode *)malloc(sizeof(InternalNode));
    newNode->isPointingToLeaf = false;
    newNode->children = malloc(sizeof(int) * ((ramificationFactor * 2) + 1));
    newNode->nodeSearch = _internalNodeSearch;
    newNode->nodeInsert = _internalNodeInsert;
    newNode->nodeRemove = _internalNodeRemove;
    newNode->nodeGet = _internalNodeGet;
    newNode->nodeLenght = _internalNodeLenght;
    newNode->nodeSizeInBytes = _internalNodeSizeInBytes;
}
static void *_internalNodeSearch(void *this, int id);
static Node *_internalNodeInsert(void *this, void *info);
static Node *_internalNodeRemove(void *this, int id);
static Node *_internalNodeGet(void *this, int i);
static int _internalNodeLenght(void *this);
static int _internalNodeSizeInBytes(void *this);
