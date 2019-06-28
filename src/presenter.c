#include <stdio.h>
#include <presenter.h>

static int showSubMenu(int option)
{
    int answer = 0;
    while (1)
    {
        switch (option)
        {
        case 2:
            printf("#########################\n");
            printf("#          MENU         #\n");
            printf("#########################\n");
            printf("# [1] Remover por ID    #\n");
            printf("# [2] Remover categoria #\n");
            printf("# [666] Voltar          #\n");
            printf("#########################\n");
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
            printf("#########################\n");
            printf("#          MENU         #\n");
            printf("#########################\n");
            printf("# [1] Listar todas      #\n");
            printf("# [2] Buscar por ID     #\n");
            printf("# [3] Listar categoria  #\n");
            printf("# [666] Voltar          #\n");
            printf("#########################\n");
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
        printf("#########################\n");
        printf("#          MENU         #\n");
        printf("#########################\n");
        printf("# [1] Adicionar Pizza   #\n");
        printf("# [2] Remover Pizza     #\n");
        printf("# [3] Alterar Pizza     #\n");
        printf("# [4] Buscar Pizza      #\n");
        printf("# [666] Sair            #\n");
        printf("#########################\n");
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
        } else {
            printf("Escolha uma opcao valida no menu.\n");
        }
    }
}


void showInfo(void)
{
}
