#include <metadata.h>
#include <storage.h>
#include <math.h>

extern Metadata *meta;

void storeMetadata(void)
{
    FILE *f = fopen(METADATA_FILE_PATH, "wb");
    fwrite(meta, sizeof(Metadata), 1, f);
    fclose(f);
}

void loadMetadata(void)
{
    FILE *f = fopen(METADATA_FILE_PATH, "rb");
    if (meta)
        free(meta);
    meta = (Metadata *)malloc(sizeof(Metadata));
    fread(meta, sizeof(Metadata), 1, f);
    fclose(f);
}

Metadata *initMetadata(int ramificationFactor)
{
    Metadata *m = (Metadata *)malloc(sizeof(Metadata));
    m->idCounter = 0;
    m->ramificationFactor = ramificationFactor;
    m->rootIsLeaf = false;
    m->rootPosition = 0;
    m->quantityInfos = 0;
    m->treeHeight = 0;
    return m;
}

void quantityInfosUpdate(bool isInsertion)
{
    if (isInsertion)
        meta->quantityInfos++;
    else // It is remotion
        meta->quantityInfos--;
    meta->treeHeight = log(meta->quantityInfos) / log(ramificationFactor);
    storeMetadata();
}
