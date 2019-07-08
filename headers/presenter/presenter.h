#ifndef PRESENTER
#define PRESENTER

#include <utils.h>
#include <info_view.h>
/* It returns a integer based on the user input */
/*
    1 -> Add
    3 -> Update
    5 -> Remove by ID
    6 -> Remove by SecIndex
    7 -> List all Info
    8 -> Get by ID
    9 -> Get By SecIndex
    666 -> Exit
 */
int showMenu(void);

#endif
