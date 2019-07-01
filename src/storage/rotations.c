#include <rotations.h>
extern int branchingFactor;
extern InfoModel mainModel;
extern Metadata *meta;

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

static void fatherInsertion(InternalNode *father, int sonPosition, int newKey, Address newNodeAddress)
{
   for (int i = father->numberOfKeys; i > sonPosition; i--)
   {
      father->children[i + 1] = father->children[i];
      father->IDs[i] = father->IDs[i - 1];
   }
   father->children[sonPosition + 1] = newNodeAddress;
   father->IDs[sonPosition] = newKey;
}

void leafNodeDivision(Address father, int sonPosition)
{
   InternalNode *fatherNode = internalNodeLoad(father);
   LeafNode *sonNode = leafNodeLoad(fatherNode->children[sonPosition]);
   LeafNode *newLeafNode = leafNodeCreate();

   newLeafNode->prox = sonNode->prox;
   for (int i = branchingFactor; i < ((branchingFactor * 2)); i++)
      newLeafNode->info[newLeafNode->numberOfKeys++] = sonNode->info[i];

   sonNode->numberOfKeys = branchingFactor - 1;

   Address newLeafNodeAddress = leafNodeStore(newLeafNode, -1);
   sonNode->prox = newLeafNodeAddress;
   leafNodeStore(sonNode, sonPosition);

   fatherInsertion(fatherNode, sonPosition, mainModel.getId(newLeafNode->info[0]), newLeafNodeAddress);

   leafNodeFree(sonNode);
   leafNodeFree(newLeafNode);
   internalNodeFree(fatherNode);
}

void internalNodeDivision(Address father, int sonPosition)
{
   InternalNode *fatherNode = internalNodeLoad(father);
   InternalNode *sonNode = leafNodeLoad(fatherNode->children[sonPosition]);
   InternalNode *newInternalNode = internalNodeCreate();

   newInternalNode->isPointingToLeaf = sonNode->isPointingToLeaf;

   int idAux = sonNode->IDs[branchingFactor];

   for (int i = branchingFactor; i < ((branchingFactor * 2)); i++)
      newInternalNode->children[newInternalNode->numberOfKeys++] = sonNode->children[i];

   int aux = 0;
   for (int i = branchingFactor; i < ((branchingFactor * 2) - 1); i++)
   {
      newInternalNode->IDs[aux] = sonNode->IDs[i];
      aux++;
   }

   sonNode->numberOfKeys = branchingFactor - 1;

   Address newInternalNodeAddress = internalNodeStore(newInternalNode, -1);
   internalNodeStore(sonNode, sonPosition);

   fatherInsertion(&fatherNode, sonPosition, idAux, newInternalNodeAddress);

   leafNodeFree(sonNode);
   leafNodeFree(newInternalNode);
   internalNodeFree(father);
}

void leafNodeoperation3A(Address father, int sonPosition)
{
   InternalNode *nodeFather;
   nodeFather = internalNodeLoad(father);

   LeafNode *node;
   LeafNode *nodeRightBrother;
   LeafNode *nodeLeftBrother;

   Address sonAddress = nodeFather->children[sonPosition];
   node = leafNodeLoad(sonAddress);

   if (node->numberOfKeys > (branchingFactor - 1))
      return;

   bool nodeHasRightBrother = (sonPosition < (nodeFather->numberOfKeys - 1));
   bool nodeHasLeftBrother = (sonPosition > 0);

   //se o irmão a direita existir, vê se ele tem + que t chaves
   if (nodeHasRightBrother && (nodeRightBrother->numberOfKeys > (branchingFactor - 1)))
   {
      Address rightBrother = nodeFather->children[sonPosition + 1];
      ;
      nodeRightBrother = leafNodeLoad(rightBrother);

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

      leafNodeStore(nodeRightBrother, rightBrother);
      leafNodeFree(nodeRightBrother);
   }
   else if (nodeHasLeftBrother && (nodeLeftBrother->numberOfKeys > (branchingFactor - 1)))
   {
      Address leftBrother = nodeFather->children[sonPosition - 1];
      nodeLeftBrother = leafNodeLoad(leftBrother);

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

      leafNodeStore(nodeLeftBrother, leftBrother);
      leafNodeFree(nodeLeftBrother);
   }
   //salvo o nó e o seu pai de volta alterados na MS e desaloco
   internalNodeStore(nodeFather, father);
   leafNodeStore(node, sonAddress);

   internalNodeFree(nodeFather);
   leafNodeFree(node);
}
void internalNodeoperation3A(Address father, int sonPosition)
{
   InternalNode *nodeFather;
   nodeFather = internalNodeLoad(father);

   InternalNode *node;
   InternalNode *nodeRightBrother;
   InternalNode *nodeLeftBrother;

   Address sonAddress = nodeFather->children[sonPosition];
   node = internalNodeLoad(sonAddress);

   if (node->numberOfKeys > (branchingFactor - 1))
      return;

   bool nodeHasRightBrother = (sonPosition < (nodeFather->numberOfKeys - 1));
   bool nodeHasLeftBrother = (sonPosition > 0);

   if (nodeHasRightBrother && (nodeRightBrother->numberOfKeys > (branchingFactor - 1)))
   {
      Address rightBrother = nodeFather->children[sonPosition + 1];
      nodeRightBrother = internalNodeLoad(rightBrother);
      //nó recebe o ID do pai no final do vetor de de IDs
      int fatherID = nodeFather->IDs[sonPosition];
      node->IDs[node->numberOfKeys] = fatherID;
      //coloco endereço do primeiro filho do irmao da direita no final do vetor de filhos do nó
      node->children[node->numberOfKeys + 1] = nodeRightBrother->children[0];
      //incremento chaves do nó
      node->numberOfKeys++;

      //ID do pai recebe o primeiro ID do irmão da direita
      nodeFather->IDs[sonPosition] = nodeRightBrother->IDs[0];
      //ajusto as chaves e filhos para tirar o "buraco" no inicio do irmão da direita
      for (int i = 1; i < nodeRightBrother->numberOfKeys; i++)
         nodeRightBrother->IDs[i - 1] = nodeRightBrother->IDs[i];
      for (int i = 1; i < nodeRightBrother->numberOfKeys + 1; i++)
         nodeRightBrother->children[i - 1] = nodeRightBrother->children[i];
      //decremento numero de chaves do irmão da direita
      nodeRightBrother->numberOfKeys--;

      internalNodeStore(nodeRightBrother, rightBrother);
      internalNodeFree(nodeRightBrother);
   }
   else if (nodeLeftBrother && (nodeLeftBrother->numberOfKeys > (branchingFactor - 1)))
   {
      Address leftBrother = nodeFather->children[sonPosition - 1];
      nodeLeftBrother = internalNodeLoad(leftBrother);

      //ando com todos as chaves do nó e seus filhos pra direita
      for (int i = node->numberOfKeys; i > 0; i--)
         node->IDs[i] = node->IDs[i - 1];
      for (int i = node->numberOfKeys + 1; i > 0; i--)
         node->children[i] = node->children[i - 1];
      //incremento a quantidade de chaves do nó
      node->numberOfKeys++;

      //ID do pai vai para a primeira posição das chaves do nó
      int fatherId = nodeFather->IDs[sonPosition];
      node->IDs[0] = fatherId;

      //ultimo filho do irmão da esquerda vira o primeiro filho do nó
      node->children[0] = nodeLeftBrother->children[nodeLeftBrother->numberOfKeys];
      //ultima chave do irmão da esquerda se torna o ID do pai
      nodeFather->IDs[sonPosition] = nodeLeftBrother->IDs[nodeLeftBrother->numberOfKeys - 1];
      //decremento o numero de chaves do irmão da esquerda
      nodeLeftBrother->numberOfKeys--;

      internalNodeStore(nodeLeftBrother, leftBrother);
      internalNodeFree(nodeLeftBrother);
   }

   //salvo de volta o pai e o nó alterados na MS e desaloco
   internalNodeStore(nodeFather, father);
   internalNodeFree(nodeFather);

   internalNodeStore(node, sonAddress);
   internalNodeFree(node);
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

void operation3B(Address father, int sonPosition)
{
   InternalNode *nodeFather = internalNodeLoad(father);

   if (father == meta->rootPosition)
   {
      rootOperation3B(father, sonPosition);
   }
   else if (nodeFather->isPointingToLeaf)
   {
      leafNodeoperation3B(father, sonPosition);
   }
   else
   {
      internalNodeoperation3B(father, sonPosition);
   }
   internalNodeFree(nodeFather);
}

void rootOperation3B(father, sonPosition)
{
   InternalNode *root = internalNodeLoad(father);

   if (root->isPointingToLeaf)
   {
      LeafNode *son = leafNodeLoad(root->children[sonPosition]);
      if (sonPosition < root->numberOfKeys - 1) // Join with right brother
      {
         int i;
         LeafNode *rightBrother = leafNodeLoad(root->children[sonPosition + 1]);
         for (i = 0; i < rightBrother->numberOfKeys; i++)
         {
            son->info[son->numberOfKeys + i] = rightBrother->info[i];
         }
         son->prox = rightBrother->prox;
         son->numberOfKeys += rightBrother->numberOfKeys;

         for (i = sonPosition; i < root->numberOfKeys - 1; i++)
         {
            root->IDs[i] = root->IDs[i + 1];
            root->children[i + 1] = root->children[i + 2];
         }
         root->numberOfKeys--;
         leafNodeFree(rightBrother);
      }
      else if (sonPosition > 0) // Join with left brother
      {
         int i;
         LeafNode *leftBrother = leafNodeLoad(root->children[sonPosition - 1]);
         for (i = 0; i < son->numberOfKeys; i++)
         {
            leftBrother->info[leftBrother->numberOfKeys + i] = son->info[i];
         }
         leftBrother->prox = son->prox;
         leftBrother->numberOfKeys += son->numberOfKeys;

         for (i = sonPosition - 1; i < root->numberOfKeys - 1; i++)
         {
            root->IDs[i] = root->IDs[i + 1];
            root->children[i + 1] = root->children[i + 2];
         }
         sonPosition--;
         root->numberOfKeys--;
         // The son became the leftBrother.
         LeafNode *aux = son;
         son = leftBrother;
         leafNodeFree(aux);
      }
      else // If the operation isn't possible...
      {
         internalNodeFree(root);
         leafNodeFree(son);
         return;
      }

      if (root->numberOfKeys == 0)
      {
         meta->rootPosition = root->children[0];
         meta->rootIsLeaf = true;
         storeMetadata();
      }
   }
   else
   {
      InternalNode *son = internalNodeLoad(root->children[sonPosition]);
      if (true) // TODO
      {
      }
   }
}

void leafNodeoperation3B(Address father, int sonPosition)
{
   InternalNode *nodeFather;
   nodeFather = internalNodeLoad(father);

   LeafNode *node;
   LeafNode *nodeRightBrother;
   LeafNode *nodeLeftBrother;

   Address sonAddress = nodeFather->children[sonPosition];
   node = leafNodeLoad(sonAddress);

   bool nodeHasRightBrother = (sonPosition < (nodeFather->numberOfKeys - 1));
   bool nodeHasLeftBrother = (sonPosition > 0);

   bool lBrotherCanBeJoined = (nodeHasLeftBrother && (nodeLeftBrother->numberOfKeys <= (branchingFactor - 1)));
   bool nodeCanBeJoined = (node && node->numberOfKeys <= (branchingFactor - 1));
   bool rBrotherCanBeJoined = (nodeHasRightBrother && (nodeRightBrother->numberOfKeys <= (branchingFactor - 1)));
   bool HasTheConditionsToExecute = (nodeCanBeJoined && (lBrotherCanBeJoined || rBrotherCanBeJoined));

   if (!HasTheConditionsToExecute)
      return;

   if (rBrotherCanBeJoined)
   {
      Address rightBrother = nodeFather->children[sonPosition + 1];
      nodeRightBrother = leafNodeLoad(rightBrother);

      //tras todos os iDs e infos do irmão pro final do nó
      for (int i = 0; i < nodeRightBrother->numberOfKeys; i++)
         node->info[node->numberOfKeys + i] = nodeRightBrother->info[i];
      //numero de chaves no nó aumenta no numero de chaves do irmão direito
      node->numberOfKeys += nodeRightBrother->numberOfKeys;
      //próximo do nó se torna o próximo do irmão da direita
      node->prox = nodeRightBrother->prox;

      // a partir do ID na posição sonPosition e o filho sonPosition+1 do pai, todo mundo anda pra esquerda
      for (int i = sonPosition; i < nodeFather->numberOfKeys - 1; i++)
         nodeFather->IDs[i] = nodeFather->IDs[i + 1];
      for (int i = sonPosition + 1; i < nodeFather->numberOfKeys; i++)
         nodeFather->children[i] = nodeFather->children[i + 1];

      //decremento a quantidade de chaves do pai
      nodeFather->numberOfKeys--;

      leafNodeStore(nodeRightBrother, rightBrother);
      leafNodeFree(nodeRightBrother);
   }
   else if (lBrotherCanBeJoined)
   {
      Address leftBrother = nodeFather->children[sonPosition - 1];
      nodeLeftBrother = leafNodeLoad(leftBrother);

      //anda com todas as chaves e infos do nó para a direita tantas vezes quanto o numero de chaves do irmão esquerdo
      for (int i = node->numberOfKeys - 1; i >= 0; i--)
         node->info[i + nodeLeftBrother->numberOfKeys] = node->info[i];
      //coloco os todas as chaves e filhos do irmão esquerdo na mesma posição, só que no nó
      for (int i = 0; i < nodeLeftBrother->numberOfKeys; i++)
         node->info[i] = nodeLeftBrother->info[i];
      //numero de chaves no nó aumenta no numero de chaves do irmão esquerdo
      node->numberOfKeys += nodeLeftBrother->numberOfKeys;

      //se o irmão esquerdo tiver um irmão esquerdo, o proximo dele se torna o nó(próximo do irmão esquerdo do nó)
      bool lBrotherHasLBrother = ((sonPosition - 1) > 0);

      if (lBrotherHasLBrother)
      {
         Address lBrotherOfLBrother = nodeFather->children[sonPosition - 2];
         LeafNode *lBrotherOfNodeLBrother = leafNodeLoad(lBrotherOfLBrother);
         lBrotherOfNodeLBrother->prox = nodeLeftBrother->prox;
         leafNodeStore(lBrotherOfLBrother, lBrotherOfLBrother);
         leafNodeFree(lBrotherOfNodeLBrother);
      }

      // a partir do ID na posição sonPosition e o filho sonPosition+1 do pai, todo mundo anda pra esquerda
      for (int i = sonPosition - 1; i < nodeFather->numberOfKeys - 1; i++)
         nodeFather->IDs[i] = nodeFather->IDs[i + 1];
      for (int i = sonPosition - 1; i < nodeFather->numberOfKeys; i++)
         nodeFather->children[i] = nodeFather->children[i + 1];

      //decremento a quantidade de chaves do pai
      nodeFather->numberOfKeys--;

      leafNodeStore(nodeLeftBrother, leftBrother);
      leafNodeFree(nodeLeftBrother);
   }
   //guardo o pai e o nó alterados na MS de volta e depois desaloco
   internalNodeStore(nodeFather, father);
   leafNodeStore(node, sonAddress);

   internalNodeFree(nodeFather);
   leafNodeFree(node);
}
void internalNodeoperation3B(Address father, int sonPosition)
{
   InternalNode *nodeFather = internalNodeLoad(father);

   InternalNode *node;
   InternalNode *nodeRightBrother;
   InternalNode *nodeLeftBrother;

   Address sonAddress = nodeFather->children[sonPosition];
   node = internalNodeLoad(sonAddress);

   bool nodeHasRightBrother = (sonPosition < (nodeFather->numberOfKeys - 1));
   bool nodeHasLeftBrother = (sonPosition > 0);

   bool lBrotherCanBeJoined = (nodeHasLeftBrother && (nodeLeftBrother->numberOfKeys <= (branchingFactor - 1)));
   bool nodeCanBeJoined = (node && node->numberOfKeys <= (branchingFactor - 1));
   bool rBrotherCanBeJoined = (nodeHasRightBrother && (nodeRightBrother->numberOfKeys <= (branchingFactor - 1)));
   bool HasTheConditionsToExecute = (nodeCanBeJoined && (lBrotherCanBeJoined || rBrotherCanBeJoined));

   if (!HasTheConditionsToExecute)
      return;

   if (rBrotherCanBeJoined)
   {
      //pego a chave do pai no sonPosition e coloco no final do vetor de chaves do nó
      //pego todas as chaves e filhos do irmão da direita e coloco no final do nó
      //
   }
   else if (lBrotherCanBeJoined)
   {
   }

   // guardando nó e o pai alterados na MS e desalocando
   internalNodeStore(nodeFather, father);
   internalNodeStore(node, sonAddress);

   internalNodeFree(nodeFather);
   internalNodeFree(node);
}
