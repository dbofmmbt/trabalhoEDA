#include <utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <pizza.h>
#include <info_model.h>
#include <info_view.h>

InfoModel mainModel = {
    pizzaSave,
    pizzaRead,
    pizzaSize,
    pizzaCategory,
    getNextCategoryPosition,
    setNextCategoryPosition
};

InfoView mainView = {
    pizzaPrint,
};

int main(int argc, char const *argv[])
{
    return 0;
}
