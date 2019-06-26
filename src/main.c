#include <utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <pizza.h>
#include <storage/storage.h>
#include <interfaces/info_view.h>
#include <interfaces/info_model.h>

InfoModel mainModel = {
    pizzaSave,
    pizzaRead,
    pizzaSize,
    pizzaFree,
    pizzaGetId,
    pizzaSetId,
    pizzaCategory,
    getNextCategoryPosition,
    setNextCategoryPosition
};

InfoView mainView = {
    pizzaPrint,
    pizzaName
};

int main(int argc, char const *argv[])
{
    printf("Teste\n");
    return 0;
}
