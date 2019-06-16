#include <utils.h>
#include <info_model.h>

/* The name convention in sec_index is:
    * Element: A value of the secondary index
    * Related: A info that belongs to a specific Element
*/

void addElement(char *element);

void *getAllRelated(char *element);
/* It gets all elements registered on the system */
char **getAllElementsName(void);
/* Size of the array of the element's names */
int getAllElementsCount(void);

void deleteAllRelated(char *element);