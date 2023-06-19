#include "DemoCmd.h"

//  בודק אם יש פייפ בפקודה מהמשתמש ומפצל לחלקים ומכניס כל חלק לתא במערך
int Check_Pipe(char* str, char** pipeParts) 
{ 
    for (int i = 0; i < 2; i++) { 
        //מחלק את הפקודה לפי הפייפים
        pipeParts[i] = strsep(&str, "|"); 
        if (pipeParts[i] == NULL) 
            break; 
    } 

    if (pipeParts[1] == NULL) 
        return 0;
    else { 
        return 1; 
    } 
} 

// מוחק רווחים מהפקודות ומכניס רק את הפקודות בתאים במערך
void CleanSpace(char* str, char** command) 
{ 
    for (int i = 0; i < 1000; i++) { 
        command[i] = strsep(&str, " "); 
        if (command[i] == NULL) 
            break; 
        if (strlen(command[i]) == 0) 
            i--; 
    } 
} 


// מבצע פירסור לפקודה שהמשתמש הכניס
parseInfo * Parse(char* cmdLines) {
        
        // הקצאת זיכרון למשתנה ששומר את הפוקדה מהמשתמש 
        parseInfo *info = malloc(sizeof(parseInfo));
        if(info == NULL) {
             return NULL;
        }
          
        char* pipeParts[2];
        
        //  בודק אם קיימים פייפים בקוד במידה וכן אז הדגל יהיה על 1
        info->pipeFlag = Check_Pipe(cmdLines, pipeParts); 
        if (info->pipeFlag) { 
                CleanSpace(pipeParts[0], info->args); 
                CleanSpace(pipeParts[1], info->argsPipes); 
        } else { 
                CleanSpace(cmdLines, info->args); 
        }

        if (RunCustomCommand(info->args)) {
            //שינוי הדגל של הפייפ כדי שנוכל להמשיך בלואלה
            info->pipeFlag = -1;
        }
        return info; 
}

// במידה ואנחנו רוצים לצאת מהתוכנית או לעבור לנתיב אחר
int RunCustomCommand(char** command) 
{ 
    
    char cwd[256];
    if (strcmp(command[0],"exit") == 0) { 
         
        printf("\nExit...\n"); 
        exit(0); 
    } else if(strcmp(command[0],"cd") == 0) {
         
        chdir(command[1]); 
        return 1; 
    }
    else if(strcmp(command[0],"tree") == 0){
    
        getcwd(cwd, sizeof(cwd));
        print_tree(cwd,0);
        return 1; 
    }
    
   
    return 0; 
} 

void LsToFile(char** command){

    char buffer[BUFFER_SIZE];
    char result[BUFFER_SIZE] = "";
    FILE* pipe = popen("ls -l", "r");
    if (!pipe) {
        printf("Failed to run command\n");
        return 1;
    }
    while (fgets(buffer, BUFFER_SIZE, pipe)) {
        strcat(result, buffer);
    }
    pclose(pipe);
    
    FILE* file = fopen(command[3], "w");
    if (file == NULL) {
        printf("Failed to open file\n");
        return 1;
    }
    fprintf(file,result);
    fclose(file);

}

//הדפסה של העץ 
 void print_tree(const char *path, int level) {
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(path))) {
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char subpath[1024];
            int len = snprintf(subpath, sizeof(subpath)-1, "%s/%s", path, entry->d_name);
            subpath[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            printf("%*s%s/\n", level*2, "", entry->d_name);
            print_tree(subpath, level + 1);
        } else {
            printf("%*s- %s\n", level*2, "", entry->d_name);
        }
    }

    closedir(dir);
}

// הרצת הפקודה בלי פייפ
void RunCommand(char** command) 
{ 
    if(strcmp(command[0],"cat") == 0 && strcmp(command[1],">") == 0){       
        CatCommand(command);
    }

    else if(command[2] != NULL && strcmp(command[0],"ls") == 0){
        if(strcmp(command[2],">") == 0 ){       
            LsToFile(command);  
        }
    }
    
    else if(execvp(command[0], command) < 0) { 
        printf("\nCommand not found!\n"); 
    } 
    exit(0); 
}

int Ifexist(char* filename){
    struct stat st;
    if (stat(filename, &st) == 0) {
        return 1;
    } else {
        return 0;
    }
 }

void CatCommand(char** command){
    if(Ifexist(command[2]) == 1){
        printf("the file is alredy exists\n"); 
    }
    else{             
        FILE* fp = fopen(command[2], "w");
        int fd = fileno(fp);
        dup2(fd, STDOUT_FILENO);        
    }
}

// הרצת של הקפודה עם פייפ
void RunPipedCommand(char** command, char** commandPipe) {
    int pipeFds[2];  
    if (pipe(pipeFds) == -1) { 
        printf("\nPipe error"); 
        return; 
    } 

    if (fork() == 0) { 
        close(pipeFds[READ]); 
        dup2(pipeFds[WRITE], WRITE);
        if(strcmp(command[0],"cat") == 0 && strcmp(command[1],">") == 0){       
            CatCommand(command);
        }
        else if(command[2] != NULL && strcmp(command[0],"ls") == 0){
                if(strcmp(command[2],">") == 0 ){       
                    LsToFile(command);  
                }
            }
        else{
            execvp(command[0], command);
        }
        
    }
    if (fork() == 0) { 
        close(pipeFds[WRITE]); 
        dup2(pipeFds[READ], READ);
        if(strcmp(commandPipe[0],"cat") == 0 && strcmp(commandPipe[1],">") == 0){       
            CatCommand(commandPipe);
        }
        else if(command[2] != NULL && strcmp(command[0],"ls") == 0){
                if(strcmp(commandPipe[2],">") == 0 ){       
                    LsToFile(commandPipe);  
                }
            }
        else{
            execvp(commandPipe[0], commandPipe);
        }
       
    }
    exit(0);
}

// הרצאת הפקודה עם פייפ או בלי במידה והדגל על -1 ממשיכים בלואלה האלה
void ExecuteCommand(parseInfo * info) {
        if (info->pipeFlag == 1) {
            RunPipedCommand(info->args, info->argsPipes); 
        }
        else if(info->pipeFlag == 0) {
          
            RunCommand(info->args); 
        }
}

