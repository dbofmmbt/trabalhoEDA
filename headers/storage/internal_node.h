#ifndef INTERNAL_NODE
#define INTERNAL_NODE

#include <utils.h>
#include <info_model.h>

typedef struct IN
{
    int numberOfKeys;
    int *IDs;
    Address *children;
} InternalNode;

//cria um nó interno na MP e retorna um ponteiro pra ele
InternalNode *internalNodeCreate();

/*guarda um nó no arquivo de indices e retorna o endereço onde foi salvo no arquivo.
para inserir no final do arquivo, passe como parâmetro -1 para a Posição de inserção*/
Address internalNodeStore(InternalNode *node, Address insertPosition);

//carrega um nó interno pra memória principal e retorna um ponteiro pra ele na MP
InternalNode *internalNodeLoad(Address nodePosition);

//libera um nó na MP
void internalNodeFree(InternalNode *node);

#endif
