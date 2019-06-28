#include <rotations.h>
extern int branchingFactor;
extern InfoModel mainModel;
/*
    There's an explanation about the following operations for b-trees, 3A and 3B, in portuguese:

    3) Se K NÃO está em X, descubra o filho F onde K
       deve estar. Se F tem T - 1 chaves, ou aplique
       (3A) ou aplique (3B):

        3A) Se F tem T - 1 chaves, mas possuir irmão
           imediato I com pelo menos T chaves, forneça
           uma chave de I ao pai e forneça a chave do
           pai para F.

        3B) Se F tem T - 1 chaves e o(s) irmão(s) tem
           T - 1 chaves, faça a intercalação de F com
           UM de seus irmãos imediatos, pegando uma
           chave do pai para o nó intercalado.
*/

void leafNodeDivision(Address father, int sonPosition);

void internalNodeDivision(Address father, int sonPosition);

void leafNodeoperation3A(Address father, int sonPosition)
{
   InternalNode *nodeFather = NULL;
   LeafNode *node = NULL;
   LeafNode *nodeRightBrother = NULL;
   LeafNode *nodeLeftBrother = NULL;

   nodeFather = internalNodeLoad(father);
   Address sonAddress = nodeFather->children[sonPosition];
   node = leafNodeLoad(sonAddress);

   if (node->numberOfKeys > (branchingFactor - 1))
      return;

   Address leftBrother = nodeFather->children[sonPosition - 1];
   Address rightBrother = nodeFather->children[sonPosition + 1];

   //se existir irmão a direita, ele seta
   if (sonPosition < (nodeFather->numberOfKeys - 1))
      nodeRightBrother = leafNodeLoad(rightBrother);
   //se existir irmão a esquerda, ele seta
   else if (sonPosition > 0)
      nodeLeftBrother = leafNodeLoad(leftBrother);

   //se o irmão a direita existir, vê se ele tem + que t chaves
   if (nodeRightBrother && (nodeRightBrother->numberOfKeys > (branchingFactor - 1)))
   {
      void *receivedInfo = nodeRightBrother->info[0];
      //posição mais a direita do nó recebe a info mais a esquerda do irmão da direita
      node->info[node->numberOfKeys] = receivedInfo;
      node->numberOfKeys++;

      //tira primeiro filho do irmão a direita e anda com as outras infos pra esquerda (tapando o buraco)
      for (int i = 1; i < nodeRightBrother->numberOfKeys; i++)
         nodeRightBrother->info[i - 1] = nodeRightBrother->info[i];
      //decrementa numero de chaves do irmão a direita
      nodeRightBrother->numberOfKeys--;
      //pega o id do primeiro filho do irmão da direita e poe no pai
      int newFatherID = mainModel.getId((nodeRightBrother->info[0]));
      nodeFather->IDs[sonPosition + 1] = newFatherID;
   }
   else if (nodeLeftBrother && (nodeLeftBrother->numberOfKeys > (branchingFactor - 1)))
   {
      //anda com todos os filhos do nó para a direita
      for (int i = node->numberOfKeys; i > 0; i--)
         node->info[i] = node->info[i - 1];
      //pego o ultimo filho do irmão a esquerda e colocar como primeiro filho do nó
      node->info[0] = nodeLeftBrother->info[nodeLeftBrother->numberOfKeys];

      //numero de chaves do nó ++
      node->numberOfKeys++;
      //decrementar o numero de chaves do irmão a esquerda
      nodeLeftBrother->numberOfKeys--;
      //pegar o id do primeiro filho do nó e colocar no pai
      int newFatherID = mainModel.getId((node->info[0]));
      nodeFather->IDs[sonPosition] = newFatherID;
   }
   //se existiram os nós, salvo de volta alterados na MS e desaloco
   if (nodeFather)
   {
      internalNodeStore(nodeFather, father);
      internalNodeFree(nodeFather);
   }
   if (node)
   {
      leafNodeStore(node, sonAddress);
      LeafNodeFree(node);
   }

   if (nodeRightBrother)
   {
      leafNodeStore(nodeRightBrother, rightBrother);
      LeafNodeFree(nodeRightBrother);
   }

   if (nodeLeftBrother)
   {
      leafNodeStore(nodeLeftBrother, leftBrother);
      LeafNodeFree(nodeLeftBrother);
   }
}
void internalNodeoperation3A(Address father, int sonPosition)
{
}

void operation3A(Address father, int sonPosition)
{
   InternalNode *nodeFather = internalNodeLoad(father);
   if (nodeFather->isPointingToLeaf)
   {
      leafNodeoperation3A(father, sonPosition);
   }
   else
   {
      internalNodeoperation3A(father, sonPosition);
   }
   internalNodeFree(nodeFather);
}

void operation3B(Address father, int sonPosition);
