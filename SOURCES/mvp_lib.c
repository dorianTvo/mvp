#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "mvp_lib.h"

//getFileNameFromPath permet de récupérer le nom d'un fichier à partir de son chemin
char* getWithoutSlashFromPath(char* path)
{
    for(size_t i = strlen(path) - 1; i; i--)
    {
        if(path[i] == '/')
            return &path[i+1];
    }
    return path;
}

char* getWithSlashFromPath(char* path)
{
    for(size_t i = strlen(path) - 2; i; i--)
    {
        if(path[i] == '/')
            return &path[i+1];
    }
    return path;
}

int mvOrCpOneFile(char* Spath, char* Dpath, int choice) //choice = 0 -> mv, choice = 1 -> cp
{
    unsigned int sec;
    time_t now;
    struct tm *tm;
    unsigned long lengthOfFile, counter;
    float speed, percentage, beforePercentage;
    unsigned char buff[8192];
    size_t n, m;
    char *fileName, *DPathWithFN;
    FILE *Sfile, *Dfile;

    Sfile = fopen(Spath, "rb+");

    if(Dpath[strlen(Dpath) - 1] == '/')
    {
        fileName = getWithoutSlashFromPath(Spath);
        DPathWithFN = concatenation(Dpath, fileName);
        Dfile = fopen(DPathWithFN, "w");
        free(DPathWithFN);
    }
    else
    { 
        Dfile = fopen(Dpath, "wb");
    }

    if(Sfile != NULL) 
    {
        fseek(Sfile, 0, SEEK_END);
        lengthOfFile = ftell(Sfile);
        fseek(Sfile, 0, SEEK_SET);

        sec = 0;
        beforePercentage = 0;
        percentage = 0;
        counter = 0;

        do
        {

            n = fread(buff, 1, sizeof buff, Sfile);
            if(n)
            {
                m = fwrite(buff, 1, n, Dfile);

                now = time(0); // Récupération du temps
                if ((tm = localtime (&now)) == NULL) 
                {
                    printf("Error extracting time\n");
                    return 1;
                } 

                if(tm->tm_sec != sec) // Si le temps différent du temps précédent
                {
                    speed = ((percentage - beforePercentage)*0.01*lengthOfFile)/1000000;
                    beforePercentage = percentage;
                    
                    now = time(0);
        
                    if ((tm = localtime (&now)) == NULL) 
                    {
                        printf("Error extracting time\n");
                        return 1;
                    }   

                    sec = tm->tm_sec;
                }
                counter++;
                percentage = ((float)(8192*counter)/lengthOfFile)*100;
                   
                printf("\33[2K\rProgression %.2f %% at %.2f Mo/s", percentage, speed);
                fflush(stdout);
            }
            else
                m = 0;
        } while ((n > 0) && (n == m));

        if(m)
            perror("copy");


        fclose(Dfile);
        fclose(Sfile);

        if(choice == 0)
            remove(Spath);

        return 0;
    }
    else
    {
        printf("The source file doesn't exist or isn't accessible");
        return 1;
    }
}

int mvOrCpOneFolder(char* Spath, char* Dpath, int choice)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(Spath);
    
    if(d)
    {
        struct stat st = {0};
        if(stat(Dpath, &st) == -1)
        {
            mkdir(Dpath, 0755);
        }
        else
        {
            if(Dpath[strlen(Dpath) - 1] != '/')
                Dpath = concatenation(concatenation(Dpath, "/"), getWithSlashFromPath(Spath));
            else
                Dpath = concatenation(Dpath, getWithSlashFromPath(Spath));

            mkdir(Dpath, 0755); //create the directory with the original name Spath
        }
        

        while((dir = readdir(d)) != NULL)
        {
            if(dir->d_type == DT_DIR && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
            {
                struct stat st = {0};
                char *SpathWD = concatenation(concatenation(Spath, dir->d_name), "/");
                char *DpathWD = concatenation(concatenation(Dpath, dir->d_name), "/");
                mvOrCpOneFolder(SpathWD, DpathWD, choice);

                if(choice == 0)
                    rmdir(SpathWD);
            }
            else if(dir->d_type == DT_REG)
            {
                char *SpathWF = concatenation(Spath, dir->d_name);
                char *DpathWF = concatenation(Dpath, dir->d_name);
                printf("\nMoving %s \n", dir->d_name);
                mvOrCpOneFile(SpathWF, DpathWF, choice);
            }
        }
    }

    closedir(d);

    if(choice == 0)
        rmdir(Spath);

    return 0;
}

char* concatenation(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
