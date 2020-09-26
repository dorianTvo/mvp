#include "mvp_lib.h"


int main(int argc, char *argv[]) // Fonction lancée au démarrage du script
{
    if(argc < 4) //Si l'utilisateur n'éxecute pas correctement la commande mvp
    {
        printf("\n\n\nUse of mvp command: \n");
        printf("The mvp command has the same function of \"mv or cp\" command but with a proogress bar and the writing flow\n");
        printf("mvp [--copy/--move] [-R/--recursive/null] [path of source file] [path of destination or destination+filename]\n\n");
        printf("/!\\ If you need to move an entire folder you can use -R or --recursive argument\n\n\n");
        return 0;
    }
    else if(argc == 5)
    {   
        if(strcmp("-R", argv[2]) == 0 || strcmp("--recursive", argv[2]) == 0)
        {  
            if(strcmp("--copy", argv[1]) == 0)
                mvOrCpOneFolder(argv[3], argv[4], 1);
            else if(strcmp("--move", argv[1]) == 0)
                mvOrCpOneFolder(argv[3], argv[4], 0);
        }
    }
    else
    {
        if(strcmp("--copy", argv[1]) == 0)
            mvOrCpOneFile(argv[2], argv[3], 1);
        else if(strcmp("--move", argv[1]) == 0)
            mvOrCpOneFile(argv[2], argv[3], 0);
    }

    printf("\n");
    return 0;
    
}
