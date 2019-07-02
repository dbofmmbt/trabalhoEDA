#include <storage.h>
#include <stdarg.h>

extern int branchingFactor;
extern Metadata *meta;
extern InfoModel mainModel;

// Constants for getPossibles
#define GET 0
#define POST 1
#define DELETE 2

static Address getPossibleFatherAddress(int id, int action);
static Address getPossibleLeafAddress(int id, int action);
static Address getInfoAddress(int id);

static void *loadRoot(void);

void treeWidthPrint();
void printLeafNodes();

void setupStorage(char *catalogName, int degree)
{
    branchingFactor = degree;
    FILE *f = fopen(METADATA_FILE_PATH, "rb");
    if (f) // If it exists, there's no need to setup the Store.
    {
        loadMetadata();
        fclose(f);
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
    int id = ++meta->idCounter;
    mainModel.setId(info, id);

    Address leafAddress = getPossibleLeafAddress(id, POST);
    LeafNode *leaf = leafNodeLoad(leafAddress);

    int i = 0, j;
    while (i < leaf->numberOfKeys && id > mainModel.getId(leaf->info[i]))
        i++;
    for (j = leaf->numberOfKeys; j > i; j--)
        leaf->info[j] = leaf->info[j - 1];
    leaf->info[i] = info;
    leaf->numberOfKeys++;
    leafNodeStore(leaf, leafAddress);
    leafNodeFree(leaf);

    meta->quantityInfos++;
    storeMetadata();
}

void *removeFromTree(int id)
{
    Address leafAddress = getPossibleLeafAddress(id, DELETE);
    LeafNode *leaf = leafNodeLoad(leafAddress);

    // At this point, we got the leaf.
    int i = 0, j;
    while ((i < leaf->numberOfKeys) && (id != mainModel.getId(leaf->info[i])))
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

    FILE *leafFile = fopen(DATA_FILE_PATH, "rb+");
    if (!leafFile)
    {
        return false;
    }
    fseek(leafFile, infoAddress, SEEK_SET);
    mainModel.infoSaver(info, leafFile);
    fclose(leafFile);
    return true;
    //substituir a informação antiga no arquivo pela nova (com excessão do ID)
}

static Address getInfoAddress(int id)
{
    Address leafAddress = getPossibleLeafAddress(id, GET);
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
    Address currentNodeAddress = getPossibleLeafAddress(1, GET);
    do
    {
        LeafNode *leaf = leafNodeLoad(currentNodeAddress);
        for (int i = 0; i < leaf->numberOfKeys; i++)
        {
            callback(leaf->info[i]);
        }
        currentNodeAddress = leaf->next;
        leafNodeFree(leaf);
    } while (currentNodeAddress != -1);

    return NULL;
}

void *printAllFromSecIndex(void (*callback)(void *), void *secIndex)
{
    Address currentNodeAddress = getPossibleLeafAddress(1, GET);
    int categoryExist = 0;
    do
    {
        LeafNode *leaf = leafNodeLoad(currentNodeAddress);
        for (int i = 0; i < leaf->numberOfKeys; i++)
        {
            //TODO para melhorar a generaização SecIndex deve ter a possibilidade de ser de outros tipos além de string
            if (!strcmp(mainModel.getSecIndex(leaf->info[i]), secIndex))
            {
                callback(leaf->info[i]);
                categoryExist++;
            }
        }
        currentNodeAddress = leaf->next;
        leafNodeFree(leaf);
    } while (currentNodeAddress != -1);
    if (!categoryExist)
    {
        printf("A categoria %s nao existe.\n", (char *)secIndex);
    }
    return NULL;
}

typedef struct listIDs
{
    int ID;
    struct listIDs *next;
} ListIDs;

void *removeAllFromSecIndex(void *secIndex)
{
    Address currentNodeAddress = getPossibleLeafAddress(1, GET);
    ListIDs *list = NULL;

    do
    {
        LeafNode *leaf = leafNodeLoad(currentNodeAddress);
        for (int i = 0; i < leaf->numberOfKeys; i++)
        {
            //TODO para melhorar a generaização SecIndex deve ter a possibilidade de ser de outros tipos além de string
            if (!strcmp(mainModel.getSecIndex(leaf->info[i]), secIndex))
            {
                ListIDs *newListNode = (ListIDs *)malloc(sizeof(ListIDs));
                newListNode->next = list;
                newListNode->ID = mainModel.getId(leaf->info[i]);
                list = newListNode;
            }
        }

        currentNodeAddress = leaf->next;
        leafNodeFree(leaf);

    } while (currentNodeAddress != -1);

    while (list)
    {
        void *aux = removeFromTree(list->ID);
        mainModel.infoFree(aux);
        ListIDs *tmp = list;
        list = list->next;
        free(tmp);
    }

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

static Address getPossibleFatherAddress(int id, int action)
{
    if (meta->rootIsLeaf)
        return -1;
    InternalNode *father = loadRoot();
    Address fatherAddress = meta->rootPosition;

    if (action == POST && father->numberOfKeys == 2 * branchingFactor - 1)
    {
        InternalNode *newRoot = internalNodeCreate();
        newRoot->isPointingToLeaf = false;
        newRoot->children[0] = fatherAddress;
        Address newRootAddress = internalNodeStore(newRoot, -1);
        internalNodeDivision(newRootAddress, 0);
        meta->rootPosition = newRootAddress;
        storeMetadata();
        internalNodeFree(father);
        internalNodeFree(newRoot);
        return getPossibleFatherAddress(id, action);
    }

    while (!father->isPointingToLeaf)
    {
        int i = 0;
        while (i < father->numberOfKeys && father->IDs[i] <= id)
            i++;
        Address sonAddress = father->children[i];
        InternalNode *son = internalNodeLoad(sonAddress);

        if (action == POST && son->numberOfKeys == 2 * branchingFactor - 1)
        {
            internalNodeDivision(fatherAddress, i);
        }
        else if (action == DELETE && son->numberOfKeys == branchingFactor - 1)
        {
            bool operated = false;

            if (i < father->numberOfKeys)
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
            fatherAddress = sonAddress;
            continue;
        }
        // The son needed operations. Therefore, the father must be accessed again.
        internalNodeFree(son);
        internalNodeFree(father);
        father = internalNodeLoad(fatherAddress);
    }
    internalNodeFree(father);
    return fatherAddress;
}

/* Used by search and update functions to get or change an information. */
static Address getPossibleLeafAddress(int id, int action)
{
    Address fatherAddress = getPossibleFatherAddress(id, action);

    if (fatherAddress == -1) // Treating the case where the root is a Leaf.
    {
        LeafNode *root = leafNodeLoad(meta->rootPosition);
        if (action == POST && root->numberOfKeys == 2 * branchingFactor - 1)
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
            internalNodeFree(newRoot);
            return getPossibleLeafAddress(id, action);
        }
        else
            return meta->rootPosition;
    }

    InternalNode *father = internalNodeLoad(fatherAddress);
    bool shouldFixTree = true;
    LeafNode *leaf;
    Address leafAddress;
    do
    {
        int i = 0;
        while (i < father->numberOfKeys && father->IDs[i] <= id)
            i++;
        leafAddress = father->children[i];
        leaf = leafNodeLoad(leafAddress);

        if (action == POST && leaf->numberOfKeys == 2 * branchingFactor - 1)
        {
            leafNodeDivision(fatherAddress, i);
        }
        else if (action == DELETE && leaf->numberOfKeys == branchingFactor - 1)
        {
            bool operated = false;

            if (i < father->numberOfKeys)
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

    leafNodeFree(leaf);
    return leafAddress;
}

void printTree()
{
    if (!meta->quantityInfos)
    {
        return;
    }
    if (!meta->rootIsLeaf)
    {
        treeWidthPrint();
    }
    printf("\n");
    printLeafNodes();
}

typedef struct addressList
{
    Address info;
    struct addressList *next;
    int deepness;
} AddressList;

void treeWidthPrint()
{
    Address root = meta->rootPosition;
    AddressList *list = malloc(sizeof(AddressList));
    AddressList *final = list;
    //coloco a raiz na fila
    list->info = root;
    list->next = NULL;
    list->deepness = 0;

    while (list)
    {
        Address nodeAddress = list->info;

        InternalNode *node = internalNodeLoad(nodeAddress);
        if (!node->isPointingToLeaf)
        {
            for (int i = 0; i <= node->numberOfKeys; i++)
            {
                AddressList *new = malloc(sizeof(AddressList));
                new->info = node->children[i];
                new->deepness = list->deepness + 1;
                new->next = NULL;
                final->next = new;
                final = new;
            }
        }
        printf("| ");
        for (int i = 0; i < node->numberOfKeys; i++)
        {
            printf("%d ", node->IDs[i]);
        }
        printf("|");

        AddressList *aux = list;
        list = list->next;

        if (list && (aux->deepness != list->deepness))
            printf("\n");
        free(aux);
        internalNodeFree(node);
    }
}

void printLeafNodes()
{
    LeafNode *current_leaf = leafNodeLoad(getPossibleLeafAddress(0, GET));

    LeafNode *aux;
    while (current_leaf)
    {
        printf("| ");
        for (int i = 0; i < current_leaf->numberOfKeys; i++)
        {
            printf("-");
            printf("%i", mainModel.getId(current_leaf->info[i]));
            printf("-");
        }
        printf(" |");
        aux = current_leaf;
        if (current_leaf->next != -1)
            current_leaf = leafNodeLoad(current_leaf->next);
        else
            current_leaf = NULL;
        leafNodeFree(aux);
    }
    printf("\n");
    return;
}
