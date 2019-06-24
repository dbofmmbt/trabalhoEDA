#include <utils.h>
#include <info_model.h>

typedef struct IN
{
    int numberOfKeys;
    int *IDs;
    Address *children;
} internalNode;

//cria um nó interno na MP e retorna um ponteiro pra ele
internalNode *internalNodeCreate();

/*guarda um nó no arquivo de indices e retorna o endereço onde foi salvo no arquivo.
para inserir no final do arquivo, passe como parâmetro -1 para a Posição de inserção*/
Address internalNodeStore(internalNode *node, Address insertPosition);

//carrega um nó interno pra memória principal e retorna um ponteiro pra ele na MP
internalNode *internalNodeLoad(Address nodePosition);

//libera um nó na MP
void internalNodeFree(internalNode *node);
