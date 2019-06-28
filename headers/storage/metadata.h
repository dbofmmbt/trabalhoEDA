#ifndef METADATA
#define METADATA

#include <utils.h>

typedef struct metadata
{
    bool rootIsLeaf;
    int rootPosition, idCounter, branchingFactor, quantityInfos;

} Metadata;

Metadata *meta;

void storeMetadata(void);
void loadMetadata(void);
Metadata *initMetadata(int branchingFactor);

/* When isInsertion is true, it increments info quantity. When it's false, the quantity is decremented.  */
void quantityInfosUpdate(bool isInsertion);

/* Use it to get an ID for a new record */
int getNewId(void);

#endif
