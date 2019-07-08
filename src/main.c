#include <utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <pizza.h>
#include <storage.h>
#include <interface_init.h>
#include <presenter.h>

#define UNUSED_PARAMETER(x) ((void)x)

extern Metadata *meta;

void getInitialParamsAndSetStorage(void)
{
    char fileName[100];
    int t = 0;
    printf("Digite o nome do arquivo: ");
    scanf("%s", fileName);
    while (t < 2)
    {
        printf("Digite o fator de ramificação maior que um: ");
        scanf("%d", &t);
    }
    setupStorage(fileName, t);
}

int main(int argc, char const *argv[])
{
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);

    getInitialParamsAndSetStorage();

    int menuAnswer;
    menuAnswer = showMenu();
    while (menuAnswer != 666)
    {
        switch (menuAnswer)
        {
        case 0:
        {
            int x;
            printf("Adicionar quantos elementos? ");
            scanf("%d", &x);
            for (int i = 0; i < x; i++)
            {
                Pizza *p = pizzaCreate(0, "namePizza", "categoryPizza", 10.0);
                insertOnTree(p);
            }

            forEachInfo(mainView.infoPrint);
            break;
        }
        case 1: //Adicionar pizza
        {
            char namePizza[100];
            char categoryPizza[100];
            float pricePizza;
            printf("Nome: ");
            scanf("%s", namePizza);
            printf("Categoria: ");
            scanf("%s", categoryPizza);
            do
            {
                printf("Preco: ");
                scanf("%f", &pricePizza);
                if (pricePizza < 0.0)
                {
                    printf("O valor da pizza deve ser positivo.\n");
                }

            } while (pricePizza < 0.0);

            Pizza *p = pizzaCreate(0, namePizza, categoryPizza, pricePizza);
            insertOnTree(p);
            forEachInfo(mainView.infoPrint);
            break;
        }
        case 3: //Alterar pizza
        {
            int IDPizza;
            char namePizza[100];
            char categoryPizza[100];
            float pricePizza;

            printf("ID da pizza a ser alterada: ");
            scanf("%d", &IDPizza);
            printf("Novo Nome: ");
            scanf("%s", namePizza);
            printf("Nova Categoria: ");
            scanf("%s", categoryPizza);
            do
            {
                printf("Novo Preco: ");
                scanf("%f", &pricePizza);
                if (pricePizza < 0.0)
                {
                    printf("O valor da pizza deve ser positivo.\n");
                }

            } while (pricePizza < 0.0);

            Pizza *p = pizzaCreate(IDPizza, namePizza, categoryPizza, pricePizza);
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
            char categoryPizza[100];
            printf("Categoria a ser removida: ");
            scanf("%s", categoryPizza);
            removeAllFromSecIndex(categoryPizza);
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
            }
            else
            {
                printf("O objeto %s não foi encontrado.\n", mainView.infoName(0));
            }

            break;
        }
        case 9: //Listar pizzas de uma categoria
        {
            char secIndex[100];
            printf("Categoria a ser listada: ");
            scanf("%s", secIndex);
            printf("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
            printf("|                    LISTA                    |\n");
            printf("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
            printAllFromSecIndex(pizzaPrint, secIndex);
            printf("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
            break;
        }
        case 10:
        {
            printTree();
            break;
        }
        default:
            break;
        }
        menuAnswer = showMenu();
    }
    printf("Fim do programa!\nSeja feliz! :)\nComa pizza!\n");
    return 0;
}
