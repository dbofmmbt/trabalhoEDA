#include <leaf_node.h>
#include <info_model.h>
#include <metadata.h>

extern Metadata *meta;
extern InfoModel mainModel;

LeafNode *leafNodeCreate(void)
{
    LeafNode *leaf = (LeafNode *)malloc(sizeof(LeafNode));

    leaf->numberOfKeys = 0;
    leaf->info = malloc(sizeof(void *) * meta->branchingFactor * 2 - 1);
    leaf->next = -1;

    return leaf;
}

Address leafNodeStore(LeafNode *node, Address pos)
{
    FILE *f = fopen(DATA_FILE_PATH, "rb+");
    if (pos < 0)
    {
        fseek(f, 0, SEEK_END);
        pos = ftell(f);
    }
    else
        fseek(f, pos, SEEK_SET);

    fwrite(&node->numberOfKeys, sizeof(int), 1, f);

    int i;
    for (i = 0; i < node->numberOfKeys; i++)
    {
        void *info = node->info[i];
        mainModel.infoSaver(info, f);
    }

    int maxNumberKeys = meta->branchingFactor * 2 - 1;
    fseek(f, mainModel.infoSize() * (maxNumberKeys - i), SEEK_CUR);

    fwrite(&node->next, sizeof(int), 1, f);
    fclose(f);
    return pos;
}

LeafNode *leafNodeLoad(Address pos)
{
    LeafNode *leaf = leafNodeCreate();
    FILE *f = fopen(DATA_FILE_PATH, "rb");
    fseek(f, pos, SEEK_SET);

    fread(&leaf->numberOfKeys, sizeof(int), 1, f);

    for (int i = 0; i < leaf->numberOfKeys; i++)
        leaf->info[i] = mainModel.infoLoader(f);

    int maxNumberKeys = meta->branchingFactor * 2 - 1;
    fseek(f, mainModel.infoSize() * (maxNumberKeys - leaf->numberOfKeys), SEEK_CUR);

    fread(&leaf->next, sizeof(Address), 1, f);

    fclose(f);
    return leaf;
}

void leafNodeFree(LeafNode *node)
{
    for (int i = 0; i < node->numberOfKeys; i++)
        mainModel.infoFree(node->info[i]);
    free(node->info);
    free(node);
}

Address leafNodeInfoAddress(Address nodeAddress, int index)
{
    int numberOfKeysSize = sizeof(int);
    return nodeAddress + numberOfKeysSize + index * mainModel.infoSize();
}
