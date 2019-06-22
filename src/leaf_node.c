#include <stdio.h>
#include <leaf_node.h>
#include <storage.h>
#include <info_model.h>

extern int ramificationFactor;
extern InfoModel mainModel;

struct leafNode
{
    int numberKeys;
    void **info;
    Address prox;
};

LeafNode *leafNodeCreate(int t)
{
    LeafNode *leaf = (LeafNode *)malloc(sizeof(LeafNode));

    leaf->numberKeys = 0;
    leaf->info = malloc(sizeof(void *) * ramificationFactor * 2 - 1);
    leaf->prox = -1;

    return leaf;
}

Address leafNodeStore(LeafNode *node, Address pos)
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
        void *info = ln->info[i];
        mainModel.infoSaver(info, f);
    }

    int maxNumberKeys = ramificationFactor * 2 - 1;
    fwrite("0", sizeof(char), mainModel.infoSize() * (maxNumberKeys - i), f);

    fwrite(&ln->prox, sizeof(int), 1, f);
    fclose(f);
    return pos;
}

LeafNode *leafNodeLoad(Address pos)
{
    LeafNode *leaf = leafNodeCreate(ramificationFactor);
    FILE *f = fopen(DATA_FILE_PATH, "rb");
    fseek(f, pos, SEEK_SET);

    fread(&leaf->numberKeys, sizeof(int), 1, f);

    for (int i = 0; i < leaf->numberKeys; i++)
        leaf->info[i] = mainModel.infoLoader(f);

    int maxNumberKeys = ramificationFactor * 2 - 1;
    fseek(f, mainModel.infoSize() * (maxNumberKeys - leaf->numberKeys), SEEK_CUR);

    fread(leaf->prox, sizeof(Address), 1, f);
    
    fclose(f);
    return leaf;
}
