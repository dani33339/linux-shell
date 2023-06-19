#include <unistd.h>
#include <stdio.h>
#include<string.h> 
#include<stdlib.h> 
#include<sys/wait.h> 
#include<sys/types.h> 
#include<readline/readline.h> 
#include <dirent.h>
#include <sys/stat.h>
#define clear() printf("\033[H\033[J")
#define READ 0
#define WRITE 1
#define DT_DIR 4
#define BUFFER_SIZE 1024

typedef struct ParseInfo {
    char* args[1000]; // args commands and files
    char* argsPipes[1000]; // args after pipe 
    int pipeFlag; // flag for checking existance of pipe and custom command
} parseInfo;

parseInfo * Parse(char*);
void ExecuteCommand(parseInfo *);
void printStructInfo(parseInfo *);
int Check_Pipe(char*, char**);
void CleanSpace(char*, char**); 
void RunCommand(char**);
void RunPipedCommand(char**, char**); 
int RunCustomCommand(char**);
void LsToFile(char**);
void print_tree(const char*, int);
int Ifexist(char*);
