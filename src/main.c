#include <utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <pizza.h>
#include <storage/storage.h>
#include <interfaces/info_view.h>
#include <interfaces/info_model.h>
#include <presenter/presenter.h>

InfoModel mainModel = {
    pizzaSave,
    pizzaRead,
    pizzaSize,
    pizzaFree,
    pizzaGetId,
    pizzaSetId,
    pizzaCategory,
    getNextCategoryPosition,
    setNextCategoryPosition};

InfoView mainView = {
    pizzaPrint,
    pizzaName,
    showMenu
    };

int main(int argc, char const *argv[])
{
    int menuAnswer;
    menuAnswer = showMenu();
    while (menuAnswer != 666)
    {
        switch (menuAnswer)
        {
        case 1: //Adicionar pizza
            Pizza *p;
            
            break;
        case 3: //Alterar pizza
            
            break;
        case 5: //Remover pizza por ID
            
            break;
        case 6: //Remover categoria e todas as pizza da categoria
            
            break;
        case 7: //Listar todas as pizzas
            
            break;
        case 8: //Buscar pizza por ID
            
            break;
        case 9: //Listar pizzas de uma categoria
            
            break;

        default:
            break;
        }
        menuAnswer = showMenu();
    }
    printf("Fim do programa!\nSeja feliz! :)\nComa pizza!\n");
    return 0;
}
