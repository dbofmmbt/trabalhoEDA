#include <stdio.h>
#include <leaf_node.h>
#include <storage.h>
#include <info_model.h>

extern InfoModel mainModel;

struct leafNode
{
    Node;
    void *info;
    int prox;
};

Node *leafNodeCreate(int t) {}

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
    for (int i = 0; i < ln->numberKeys; i++)
    {
        void *info = ln->nodeGet(ln, i);
        mainModel.infoSaver(info, f);
    }
    fwrite(&ln->prox, sizeof(int), 1, f);
    fclose(f);
    return pos;
}

/* These functions are used to set the function pointers on Create */
static void *_leafNodeSearch(void *this, int id);
static Node *_leafNodeInsert(void *this, void *info);
static Node *_leafNodeRemove(void *this, int id);