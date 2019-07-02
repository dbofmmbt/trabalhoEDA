#ifndef INFO_MODEL
#define INFO_MODEL

/* The Model is responsible for the data abstraction */
typedef struct infoModel
{
    /* It should write a record in a given file */
    void (*infoSaver)(void *info, FILE *file);
    /* It should load a record from a given file */
    void *(*infoLoader)(FILE *file);
    /* The size of the record in Secondary Memory */
    int (*infoSize)(void);

    void (*infoFree)(void *info);

    int (*getId)(void *info);

    void (*setId)(void *info, int id);

    void *(*getSecIndex)(void *info);

} InfoModel;

#endif
