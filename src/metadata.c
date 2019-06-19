#include <metadata.h>
#include <storage.h>

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
    if(meta)
        free(meta);
    meta = (Metadata *)malloc(sizeof(Metadata));
    fread(meta, sizeof(Metadata), 1, f);
    fclose(f);
}