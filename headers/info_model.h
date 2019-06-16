/* The Model is responsible for the data abstraction */
typedef struct infoModel
{
    /* It should write a record in a given file */
    void (*infoSaver)(void *info, FILE *file);
    /* It should load a record from a given file */
    void *(*infoLoader)(FILE *file);
    /* The size of the record in Secondary Memory */
    int (*infoSize)(void);
    
    char *(*getSecIndex)(void *info);

    int (*getNextSecIndexPosition)(void *info);

    void (*setNextSecIndexPosition)(void *info, int position);
} InfoModel;