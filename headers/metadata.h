#include <utils.h>

typedef struct metadata
{
    bool rootIsLeaf;
    int rootPosition, idCounter, ramificationFactor;

} Metadata;

Metadata *meta;

void storeMetadata(void);
void loadMetadata(void);
Metadata *initMetadata(int ramificationFactor);