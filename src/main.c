#include <utils.h>
#include <stdio.h>
#include <stdlib.h>
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
        case 0: // Just for debug purposes
        {
            int x;
            printf("Adicionar quantos elementos? ");
            scanf("%d", &x);
            for (int i = 0; i < x; i++)
            {
                void *info = mainModel.getSampleInfo();
                insertOnTree(info);
            }

            forEachInfo(mainView.infoPrint);
            break;
        }
        case 1: //Adicionar info
        {
            void *info = mainModel.getInfoFromUser();
            insertOnTree(info);
            break;
        }
        case 3: //Alterar info
        {
            int ID;
            printf("ID de %s a ser alterada(o): ", mainView.infoName(0));
            scanf("%d", &ID);

            void *info = mainModel.getInfoFromUser();
            mainModel.setId(info, ID);
            updateOnTree(info);
            break;
        }
        case 5: //Remover info por ID
        {
            int ID;
            printf("ID da(o) %s a ser removida(o): ", mainView.infoName(0));
            scanf("%d", &ID);
            void *info = removeFromTree(ID);
            printf("%s removida(o):\n", mainView.infoName(0));
            mainView.infoPrint(info);
            mainModel.infoFree(info);
            break;
        }
        case 6: //Remover SecIndex
        {
            char secIndex[100];
            printf("%s a ser removida(o): ", mainView.infoSecIndexName(0));
            scanf("%s", secIndex);
            removeAllFromSecIndex(secIndex);
            break;
        }
        case 7: //Listar todas as infos
        {
            printf("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
            printf("|                    LISTA                    |\n");
            printf("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
            forEachInfo(mainView.infoPrint);
            printf("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
        }
        break;
        case 8: //Buscar info por ID
        {
            int ID;
            printf("ID da(o) %s a ser buscada(o): ", mainView.infoName(0));
            scanf("%d", &ID);
            void *p = getFromTree(ID);
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
        case 9: //Listar infos de uma secIndex
        {
            char secIndex[100];
            printf("%s a ser listada: ", mainView.infoSecIndexName(0));
            scanf("%s", secIndex);
            printf("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
            printf("|                    LISTA                    |\n");
            printf("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
            printAllFromSecIndex(mainView.infoPrint, secIndex);
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
    printf("Fim do programa!\nSeja feliz! :)\nComa %s!\n", mainView.infoName(1));
    return 0;
}
