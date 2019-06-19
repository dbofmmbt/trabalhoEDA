#include <utils.h>

typedef struct metadata
{
    bool rootIsLeaf;
    int rootPosition, idCounter;

} Metadata;

Metadata *meta;

void storeMetadata(void);
void loadMetadata(void);