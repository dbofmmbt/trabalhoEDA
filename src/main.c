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
    menuAnswer = mainView.showMenu();
    while (menuAnswer != 666)
    {
        switch (menuAnswer)
        {
        case 1: //Adicionar pizza
        {
            char *namePizza;
            char *categoryPizza;
            float pricePizza;

            printf("Nome: ");
            scanf("%s", &namePizza);
            printf("Categoria: ");
            scanf("%s", &categoryPizza);
            printf("Preco: ");
            scanf("%f", &pricePizza);

            Pizza *p = pizzaCreate(0, namePizza, categoryPizza, pricePizza);
            insertOnTree(p);
            break;
        }
        case 3: //Alterar pizza
        {
            int IDPizza;
            char *namePizza;
            char *categoryPizza;
            float pricePizza;

            printf("ID da pizza a ser alterada: ");
            scanf("%d", &IDPizza);
            printf("Novo Nome: ");
            scanf("%s", &namePizza);
            printf("Nova Categoria: ");
            scanf("%s", &categoryPizza);
            printf("Novo Preco: ");
            scanf("%f", &pricePizza);
            Pizza *p = pizzaCreate(0, namePizza, categoryPizza, pricePizza);
            updateOnTree(p);
            break;
        }
        case 5: //Remover pizza por ID
        {
            int IDPizza;
            printf("ID da pizza a ser removida: ");
            scanf("%d", &IDPizza);
            removeFromTree(IDPizza);
            break;
        }
        case 6: //Remover categoria e todas as pizza da categoria
        {
            char *categoryPizza;
            printf("Categoria a ser removida: ");
            scanf("%s", &categoryPizza);
            removeAllFromSecIndex(removeFromTree, categoryPizza);
            break;
        }
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
        {
            int IDPizza;
            printf("ID da pizza a ser buscada: ");
            scanf("%d", &IDPizza);
            void *p = getFromTree(IDPizza);
            if (p)
            {
                mainView.infoPrint(p);
                mainModel.infoFree(p);
            }else
            {
                printf("O objeto %s não foi encontrado", mainView.infoName);
            }

            break;
        }
        case 9: //Listar pizzas de uma categoria
        {
            char *secIndex;
            printf("Categoria a ser listada: ");
            scanf("%s", &secIndex);
            printAllFromSecIndex(pizzaPrint, secIndex);
            break;
        }

        default:
            break;
        }
        menuAnswer = mainView.showMenu();
    }
    printf("Fim do programa!\nSeja feliz! :)\nComa pizza!\n");
    return 0;
}
