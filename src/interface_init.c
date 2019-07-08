#include <interface_init.h>
#include <pizza.h>

InfoModel mainModel = {
    pizzaSave,
    pizzaRead,
    pizzaSize,
    pizzaFree,
    pizzaGetId,
    pizzaSetId,
    pizzaCategory};

InfoView mainView = {
    pizzaPrint,
    pizzaName};
