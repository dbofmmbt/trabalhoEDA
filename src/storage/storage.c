#include <unistd.h>
#include <storage.h>

extern int branchingFactor;
extern Metadata *meta;
extern InfoModel mainModel;

static Address getInfoAddress(int id);
static Address getPossibleFatherAddress(int id);
static Address getPossibleLeafAddress(int id);
static void *loadRoot(void);

void setupStorage(char *catalogName, int degree)
{
    branchingFactor = degree;
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
    while ((info = mainModel.infoLoader(catalog)))
        insertOnTree(info);
    fclose(catalog);
}

void insertOnTree(void *info)
{
    loadMetadata();
    int id = meta->idCounter++;
    meta->quantityInfos++;
    mainModel.setId(info, id);

    if (meta->rootIsLeaf)
    {
        LeafNode *root = leafNodeLoad(meta->rootPosition);
        if (root->numberOfKeys == 2 * branchingFactor - 1)
        {
            InternalNode *newRoot = internalNodeCreate();
            newRoot->isPointingToLeaf = true;
            newRoot->children[0] = meta->rootPosition;
            Address newRootAddress = internalNodeStore(newRoot, -1);
            leafNodeDivision(newRootAddress, 0);
            meta->rootPosition = newRootAddress;
            meta->rootIsLeaf = false;
            storeMetadata();
            leafNodeFree(root);
            return insertOnTree(info);
        }
        else
        {
            int i = 0, j;
            while (i < root->numberOfKeys && id > mainModel.getId(root->info))
                i++;
            for (j = root->numberOfKeys; j > i; j--)
                root->info[j] = root->info[j - 1];
            root->info[i] = info;
            root->numberOfKeys++;
            leafNodeStore(root, meta->rootPosition);
            leafNodeFree(root);
            return;
        }
    }

    InternalNode *father = loadRoot();
    Address fatherAddress = meta->rootPosition;

    while (!father->isPointingToLeaf)
    {
        int i = 0;
        while (i < father->numberOfKeys && father->IDs[i] < id)
            i++;
        Address sonAddress = father->children[i];
        InternalNode *son = internalNodeLoad(sonAddress);

        if (son->numberOfKeys == 2 * branchingFactor - 1)
        {
            internalNodeDivision(fatherAddress, i);
        }
        else // If the son doesn't need operations, continue the search down the Tree.
        {
            internalNodeFree(father);
            father = son;
            fatherAddress = sonAddress;
            continue;
        }
        // The son needed operations. Therefore, the father must be accessed again.
        internalNodeFree(son);
        internalNodeFree(father);
        father = internalNodeLoad(fatherAddress);
    }

    // At this point, we got the right father.
    bool shouldFixTree = true;
    LeafNode *leaf;
    Address leafAddress;
    do
    {
        int i = 0;
        while (i < father->numberOfKeys && father->IDs[i] < id)
            i++;
        leafAddress = father->children[i];
        leaf = leafNodeLoad(leafAddress);
        if (leaf->numberOfKeys == 2 * branchingFactor - 1)
        {
            leafNodeDivision(fatherAddress, i);
        }
        else // Happens when no division was necessary.
        {
            internalNodeFree(father);
            shouldFixTree = false;
            continue;
        }
        leafNodeFree(leaf);
        internalNodeFree(father);
        father = internalNodeLoad(fatherAddress);
    } while (shouldFixTree);

    // At this point, we got the leaf.
    int i = 0, j;
    while (i < leaf->numberOfKeys && id > mainModel.getId(leaf->info[i]))
        i++;
    for (j = leaf->numberOfKeys; j > i; j--)
        leaf->info[j] = leaf->info[j - 1];
    leaf->info[i] = info;
    leaf->numberOfKeys++;
    leafNodeStore(leaf, leafAddress);
    leafNodeFree(leaf);

    storeMetadata();
}

void *removeFromTree(int id)
{
    if (meta->rootIsLeaf)
    {
        LeafNode *root = leafNodeLoad(meta->rootPosition);
        int i = 0, j;
        while (i < root->numberOfKeys && id != mainModel.getId(root->info[i]))
            i++;
        if (i == root->numberOfKeys)
        {
            leafNodeFree(root);
            return NULL;
        }
        void *info = root->info[i];
        for (j = i; j < root->numberOfKeys - 1; j++)
        {
            root->info[j] = root->info[j + 1];
        }
        root->numberOfKeys--;
        leafNodeStore(root, meta->rootPosition);
        leafNodeFree(root);
        return info;
    }

    InternalNode *father = loadRoot();
    Address fatherAddress = meta->rootPosition;

    while (!father->isPointingToLeaf)
    {
        int i = 0;
        while (i < father->numberOfKeys && father->IDs[i] < id)
            i++;
        Address sonAddress = father->children[i];
        InternalNode *son = internalNodeLoad(sonAddress);

        if (son->numberOfKeys == branchingFactor - 1)
        {
            bool operated = false;

            if (i < father->numberOfKeys - 1)
            {
                InternalNode *rightBrother = internalNodeLoad(father->children[i + 1]);
                if (rightBrother->numberOfKeys >= branchingFactor)
                {
                    operation3A(fatherAddress, i);
                    operated = true;
                }
                internalNodeFree(rightBrother);
            }
            if (i > 0 && !operated)
            {
                InternalNode *leftBrother = internalNodeLoad(father->children[i - 1]);
                if (leftBrother->numberOfKeys >= branchingFactor)
                {
                    operation3A(fatherAddress, i);
                    operated = true;
                }
                internalNodeFree(leftBrother);
            }
            if (!operated)
                operation3B(fatherAddress, i);
        }
        else // If the son doesn't need operations, continue the search down the Tree.
        {
            internalNodeFree(father);
            father = son;
            continue;
        }
        // The son needed operations. Therefore, the father must be accessed again.
        internalNodeFree(son);
        internalNodeFree(father);
        father = internalNodeLoad(fatherAddress);
    }
    // At this point, we got the father.
    bool shouldFixTree = true;
    Address leafAddress;
    LeafNode *leaf;
    do
    {
        int i = 0;
        while (i < father->numberOfKeys && father->IDs[i] < id)
            i++;
        leafAddress = father->children[i];

        leaf = leafNodeLoad(leafAddress);
        if (leaf->numberOfKeys == branchingFactor - 1)
        {
            bool operated = false;

            if (i < father->numberOfKeys - 1)
            {
                LeafNode *rightBrother = leafNodeLoad(father->children[i + 1]);
                if (rightBrother->numberOfKeys >= branchingFactor)
                {
                    operation3A(fatherAddress, i);
                    operated = true;
                }
                leafNodeFree(rightBrother);
            }
            if (i > 0 && !operated)
            {
                LeafNode *leftBrother = leafNodeLoad(father->children[i - 1]);
                if (leftBrother->numberOfKeys >= branchingFactor)
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
            internalNodeFree(father);
            shouldFixTree = false;
            continue;
        }
        internalNodeFree(father);
        leafNodeFree(leaf);
        father = internalNodeLoad(fatherAddress);
    } while (shouldFixTree);

    // At this point, we got the leaf.
    int i = 0, j;
    while (i < leaf->numberOfKeys && id != mainModel.getId(leaf->info[i]))
        i++;
    if (i == leaf->numberOfKeys)
    {
        leafNodeFree(leaf);
        return NULL;
    }
    void *info = leaf->info[i];
    for (j = i; j < leaf->numberOfKeys - 1; j++)
    {
        leaf->info[j] = leaf->info[j + 1];
    }
    leaf->numberOfKeys--;
    leafNodeStore(leaf, leafAddress);
    leafNodeFree(leaf);

    meta->quantityInfos--;
    storeMetadata();
    return info;
}

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

static Address getInfoAddress(int id)
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

//search a node by his ID , load the node to the MP and return the pointer to where it is
void *getFromTree(int id)
{
    Address infoAdress = getInfoAddress(id);
    if (infoAdress != -1)
    {
        FILE *f = fopen(DATA_FILE_PATH, "rb");
        fseek(f, infoAdress, SEEK_SET);
        void *info = mainModel.infoLoader(f);
        fclose(f);
        return info;
    }
    return NULL;
}

// Applies the given function on each information stored on the Tree
void *forEachInfo(void (*callback)(void *))
{
    Address currentNodeAddress = getPossibleLeafAddress(1);
    do
    {
        LeafNode *leaf = leafNodeLoad(currentNodeAddress);
        for (int i = 0; i < leaf->numberOfKeys; i++)
        {
            callback(leaf->info[i]);
        }
        currentNodeAddress = leaf->prox;
        leafNodeFree(leaf);
    } while (currentNodeAddress != -1);

    return NULL;
}

/*
    The Root node could be a leaf or an internal node. The user of this function
    will have to cast it correctly by checking the meta information about the root.
 */
static void *loadRoot(void)
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
static Address getPossibleFatherAddress(int id)
{
    if (meta->rootIsLeaf)
        return -1;
    InternalNode *father = loadRoot();
    Address fatherAddress = meta->rootPosition;

    while (!father->isPointingToLeaf)
    {
        int i = 0;
        while (i < father->numberOfKeys && father->IDs[i] < id)
            i++;
        Address sonAddress = father->children[i];
        InternalNode *son = internalNodeLoad(sonAddress);

        internalNodeFree(father);
        father = son;
    }
    internalNodeFree(father);
    return fatherAddress;
}

/* Used by search and update functions to get or change an information. */
static Address getPossibleLeafAddress(int id)
{
    Address fatherAddress = getPossibleFatherAddress(id);

    if (fatherAddress == -1) // Treating the case where the root is a Leaf.
        return meta->rootPosition;

    InternalNode *father = internalNodeLoad(fatherAddress);
    int i = 0;
    while (i < father->numberOfKeys && father->IDs[i] < id)
        i++;
    Address leafAddress = father->children[i];
    internalNodeFree(father);
    return leafAddress;
}
