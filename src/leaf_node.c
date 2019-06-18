#include <stdio.h>
#include <leaf_node.h>
#include <storage.h>
#include <info_model.h>

extern int ramificationFactor;
extern InfoModel mainModel;

struct leafNode
{
    Node;
    void *info;
    int prox;
};

/* These functions are used to set the function pointers on Create */
static void *_leafNodeSearch(void *this, int id);
static Node *_leafNodeInsert(void *this, void *info);
static Node *_leafNodeRemove(void *this, int id);
static Node *_leafNodeGet(void *this, int i);
// TODO: add the other functions declared on node.h

Node *leafNodeCreate(int t)
{
    LeafNode *leaf = (LeafNode *)malloc(sizeof(LeafNode));

    leaf->nodeSearch = _leafNodeSearch;
    leaf->nodeInsert = _leafNodeInsert;
    leaf->nodeRemove = _leafNodeRemove;
    leaf->nodeGet = _leafNodeGet;

    leaf->numberKeys = 0;
    leaf->info = malloc(sizeof(void *) * ramificationFactor * 2 - 1);
    int prox = -1;

    return leaf;
}

int leafNodeStore(void *node, int pos)
{
    LeafNode *ln = (LeafNode *)node;
    FILE *f = fopen(DATA_FILE_PATH, "rb+");
    if (pos < 0)
    {
        fseek(f, 0, SEEK_END);
        pos = ftell(f);
    }
    else
        fseek(f, pos, SEEK_SET);

    fwrite(&ln->numberKeys, sizeof(int), 1, f);

    int i;
    for (i = 0; i < ln->numberKeys; i++)
    {
        void *info = ln->nodeGet(ln, i);
        mainModel.infoSaver(info, f);
    }
    
    int maxNumberKeys = ramificationFactor * 2 - 1;
    fwrite("0", sizeof(char), mainModel.infoSize() * (maxNumberKeys - i), f);

    fwrite(&ln->prox, sizeof(int), 1, f);
    fclose(f);
    return pos;
}

Node *leafNodeLoad(int pos)
{
    LeafNode *leaf = leafNodeCreate(ramificationFactor);
    FILE *f = fopen(DATA_FILE_PATH, "rb");
}
