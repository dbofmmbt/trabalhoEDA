#include <utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <pizza.h>
#include <storage.h>
#include <info_view.h>
#include <info_model.h>
#include <presenter.h>

#define UNUSED_PARAMETER(x) ((void)x)

extern Metadata *meta;

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
    showMenu};

int main(int argc, char const *argv[])
{
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);
    int menuAnswer;
    menuAnswer = showMenu();
    while (menuAnswer != 666)
    {
        switch (menuAnswer)
        {
        case 1: //Adicionar pizza
        {
            Pizza *p;
            UNUSED_PARAMETER(p);
            break;
        }
        case 3: //Alterar pizza

            break;
        case 5: //Remover pizza por ID

            break;
        case 6: //Remover categoria e todas as pizza da categoria
            int id;
            printf("Entre com o ID a ser buscado\nR: ");
            scanf("%d", &id);
            void* v = getFromTree(id);
            mainView.infoPrint(v);
            mainModel.infoFree(v);
            break;
        case 7: //Listar todas as pizzas
        {
            printf("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
            printf("|                    LISTA                    |\n");
            printf("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
            forEachInfo(pizzaPrint);
            printf("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
        }
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
