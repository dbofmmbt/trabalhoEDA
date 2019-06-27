#ifndef SEC_INDEX_USER
#define SEC_INDEX_USER

#include <utils.h>
#include <info_model.h>

/* The name convention in sec_index is:
    * Element: A value of the secondary index
    * Related: A info that belongs to a specific Element
*/

void *getAllRelated(char *element); // TODO

void deleteAllRelated(char *element); // TODO

#endif