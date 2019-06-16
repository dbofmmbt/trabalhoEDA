/* The View is responsible for the visualization abstraction */
typedef struct infoView
{
    /* It should print a record in stdout */
    void (*infoPrint)(void *info);
} InfoView;