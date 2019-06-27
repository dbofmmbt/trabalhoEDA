#include <unistd.h>
#include <storage.h>

extern int ramificationFactor;
extern Metadata *meta;
extern InfoModel mainModel;

void setupStorage(char *catalogName, int degree)
{
    ramificationFactor = degree;
    if (access(METADATA_FILE_PATH, F_OK)) // If it exists, there's no need to setup the Store.
    {
        loadMetadata();
        return;
    }
    meta = initMetadata(degree);
    storeMetadata();

    // Initializing empty files
    FILE *internalFile, *leafFile;
    internalFile = fopen(MAIN_INDEX_FILE_PATH, "wb");
    leafFile = fopen(DATA_FILE_PATH, "wb");
    fclose(internalFile);
    fclose(leafFile);

    LeafNode *root = leafNodeCreate(degree);
    leafNodeStore(root, -1);

    FILE *catalog = fopen(catalogName, "rb");
    if (!catalog)
        return;
    void *info;
    while (info = mainModel.infoLoader(catalog))
        insertOnTree(info);
    fclose(catalog);
}

void insertOnTree(void *info); // TODO

void *removeFromTree(int id); // TODO

bool updateOnTree(void *info)
{
    //recebe um ponteiro pra informação na MP
    int infoId = mainModel.getId(info);
    if (!infoId)
    {
        return false;
    }
    //usa o getId do info manager e consegue o id do cara na arvore
    int infoAddress = getInfoAddress(infoId);
    if (infoAddress == -1)
    {
        return false;
    }
    //usa o getInfoAddress com esse ID para conseguir o endereço da info no arquivo

    FILE *leafFile = fopen(DATA_FILE_PATH, "wb");
    if (!leafFile)
    {
        return false;
    }
    fseek(leafFile, infoAddress, SEEK_SET);
    fwrite(info, mainModel.infoSize(), 1, leafFile);
    fclose(leafFile);
    return true;
    //substituir a informação antiga no arquivo pela nova (com excessão do ID)
}

Address getInfoAddress(int id)
{
    Address leafAddress = getPossibleLeafAddress(id);
    LeafNode *leaf = leafNodeLoad(leafAddress);
    int i;
    for (i = 0; i < leaf->numberOfKeys; i++)
        if (mainModel.getId(leaf->info[i]) == id)
            break;
    if (i == leaf->numberOfKeys)
        return -1;
    Address infoAddress = leafNodeInfoAddress(leafAddress, i);
    leafNodeFree(leaf);
    return infoAddress;
    //recebe um ID e retorna o endereço dele no arquivo
}

void *getFromTree(int id) // TODO
{
    //search a node by his ID , load the node to the MP and return the pointer to where it is
}

void *getAllFromTree(void); // TODO

/*
    The Root node could be a leaf or an internal node. The user of this function
    will have to cast it correctly by checking the meta information about the root.
 */
void *loadRoot(void)
{
    void *root;
    Address rootAddress = meta->rootPosition;

    if (meta->rootIsLeaf)
        root = leafNodeLoad(rootAddress);
    else
        root = internalNodeLoad(rootAddress);
    return root;
}

/*
    Used by insertion and remotion functions in order to perform rotations when it's necessary.

    It will rotate the tree in the process, if needed.
*/
Address getPossibleFatherAddress(int id) // TODO: it needs to check for rotations while going through the Tree.
{
    if (meta->rootIsLeaf)
        return -1;
    InternalNode *currentNode = loadRoot();
    Address nodeAddress = meta->rootPosition;
    while (!currentNode->isPointingToLeaf)
    {
        int i = 0;
        while (i < currentNode->numberOfKeys && currentNode->IDs[i] < id)
            i++;
        InternalNode *aux = currentNode;
        nodeAddress = currentNode->children[i];
        currentNode = internalNodeLoad(nodeAddress);
        internalNodeFree(aux);
    }
    return nodeAddress;
}

/* Used by search and update functions to get or change an information. */
Address getPossibleLeafAddress(int id)
{
    Address fatherAddress = getPossibleFatherAddress(id);
    Address leafAddress = -1;
    bool shouldFixTree = true;
    do
    {
        InternalNode *father = internalNodeLoad(fatherAddress);
        int i = 0;
        while (i < father->numberOfKeys && father->IDs[i] < id)
            i++;
        leafAddress = father->children[i];

        LeafNode *leaf = leafNodeLoad(leafAddress);
        if (leaf->numberOfKeys == 2 * ramificationFactor)
        {
            leafNodeFree(leaf);
            leafNodeFree(father);
            leafNodeDivision(fatherAddress, i);
        }
        else if (leaf->numberOfKeys == ramificationFactor - 1)
        {
            bool operated = false;

            if (i < father->numberOfKeys - 1)
            {
                LeafNode *rightBrother = leafNodeLoad(father->children[i + 1]);
                if (rightBrother->numberOfKeys >= ramificationFactor)
                {
                    operation3A(fatherAddress, i);
                    operated = true;
                }
                leafNodeFree(rightBrother);
            }
            if (i > 0 && !operated)
            {
                LeafNode *leftBrother = leafNodeLoad(father->children[i - 1]);
                if (leftBrother->numberOfKeys >= ramificationFactor)
                {
                    operation3A(fatherAddress, i);
                    operated = true;
                }
                leafNodeFree(leftBrother);
            }
            if (!operated)
                operation3B(fatherAddress, i);
        }
        else // Happens when no division nor operation was necessary.
        {
            shouldFixTree = false;
        }
        internalNodeFree(father);
        leafNodeFree(leaf);
    } while (shouldFixTree);

    return leafAddress;
}
