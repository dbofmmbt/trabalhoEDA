#include <unistd.h>
#include <metadata.h>
#include <info_model.h>
#include <storage.h>
#include <leaf_node.h>

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

    Node *root = leafNodeCreate(degree);
    leafNodeStore(root, -1);

    FILE *catalog = fopen(catalogName, "rb");
    if (!catalog)
        return;
    void *info;
    while (info = mainModel.infoLoader(catalog))
        insertOnTree(info);
    fclose(catalog);
}

void insertOnTree(void *info);

void *removeFromTree(int id);

bool updateOnTree(void *info)
{
    //recebe um ponteiro pra informação na MP
    int infoId = mainModel.getId(info);
    if (!infoId)
    {
        return false;
    }
    //usa o geID do info manager e consegue o id do cara na arvore
    int infoAddress = getInfoAdressFromNode(infoId);
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

Address getInfoAdressFromNode(int ID)
{
    //recebe um ID e retorna o endereço dele no arquivo
}

void *getFromTree(int id)
{
    //search a node by his ID , load the node to the MP and return the pointer to where it is
}
void *getAllFromTree(void);

Node *loadRoot(void);

/*
    ! loadNode and storeNode will possibly be deleted.
    ! They may be changed for internal and leaf functions.
*/

Node *loadNode(long pos);

void storeNode(Node *);
