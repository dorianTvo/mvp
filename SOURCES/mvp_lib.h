#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* getWithoutSlashFromPath(char* path);
char* getWithSlashFromPath(char* path);
int mvOrCpOneFile(char* Spath, char* Dpath, int choice);
int mvOrCpOneFolder(char* Spath, char* Dpath, int choice);
char* concatanation(const char *s1, const char *s2);
