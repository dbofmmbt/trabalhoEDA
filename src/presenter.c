#include <stdio.h>
#include <string.h>
#include <presenter.h>
#include <info_view.h>

extern InfoView mainView;
int menuWidth = 25;

static int showSubMenu(int option)
{
    int answer = 0;
    while (1)
    {
        switch (option)
        {
        case 2:
            for (int i = 0; i < menuWidth; i++)
                printf("#");
            printf("\n");
            printf("#          MENU         #\n");
            for (int i = 0; i < menuWidth; i++)
                printf("#");
            printf("\n");
            printf("# [1] Remover por ID    #\n");
            printf("# [2] Remover categoria #\n");
            printf("# [666] Voltar          #\n");
            for (int i = 0; i < menuWidth; i++)
                printf("#");
            printf("\n");
            printf("R: ");
            scanf("%d", &answer);
            if (answer == 666)
            {
                return 0;
            }
            else if (answer == 1 || answer == 2)
            {
                return answer + 4;
            }
            
            printf("Escolha uma opcao valida no menu.\n");
            break;

        case 4:
            for (int i = 0; i < menuWidth; i++)
                printf("#");
            printf("\n");
            printf("#          MENU         #\n");
            for (int i = 0; i < menuWidth; i++)
                printf("#");
            printf("\n");
            printf("# [1] Listar todas      #\n");
            printf("# [2] Buscar por ID     #\n");
            printf("# [3] Listar categoria  #\n");
            printf("# [666] Voltar          #\n");
            for (int i = 0; i < menuWidth; i++)
                printf("#");
            printf("\n");
            printf("R: ");
            scanf("%d", &answer);
            if (answer == 666)
            {
                return 0;
            }
            else if (answer == 1 || answer == 2 || answer == 3)
            {
                return answer + 6;
            }
            printf("Escolha uma opcao valida no menu.\n");
            break;
        default:
            printf("Escolha uma opcao valida no menu.\n");
            break;
        }
    }
}


int showMenu(void)
{
    int answer;
    while (1)
    {
        int len = strlen(mainView.infoName(0));

        for (int i = 0; i < menuWidth; i++)
            printf("#");
        printf("\n");
        printf("#          MENU         #\n");
        for (int i = 0; i < menuWidth; i++)
            printf("#");
        printf("\n");

        printf("# [1] Adicionar %s", mainView.infoName(0));
        for (int i = 0; i < (menuWidth - 17 - len); i++)
            printf(" ");
        printf("#\n");

        printf("# [2] Remover %s", mainView.infoName(0));
        for (int i = 0; i < (menuWidth - 15 - len); i++)
            printf(" ");
        printf("#\n");

        printf("# [3] Alterar %s", mainView.infoName(0));
        for (int i = 0; i < (menuWidth - 15 - len); i++)
            printf(" ");
        printf("#\n");

        printf("# [4] Buscar %s", mainView.infoName(0));
        for (int i = 0; i < (menuWidth - 14 - len); i++)
            printf(" ");
        printf("#\n");

        printf("# [666] Sair            #\n");
        for (int i = 0; i < menuWidth; i++)
            printf("#");
        printf("\n");
        printf("R: ");
        scanf("%d", &answer);
        if (answer == 1 || answer == 3 || answer == 666)
        {
            return answer;
        }
        else if (answer == 2 || answer == 4)
        {
            answer = showSubMenu(answer);
            if (answer)
                return answer;
        }
        else if(answer == 5){
            return answer*2;
        }
        else {
            printf("Escolha uma opcao valida no menu.\n");
        }
    }
}

void showInfo(void)
{
}
