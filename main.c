#include "DemoCmd.h"


int main (int argc, char **argv) { 
    int childPid, status; 
    int pipline[2];
    char * cmdLine; 
    char cwd[256];
    char ch  = '>';
    parseInfo *info; 
    clear();
    printf("---Starting Custom Shell---\n");
    printf("---------------------------\n");
    while(1) { 
       
        // הנתיב הנוכחי
        getcwd(cwd, sizeof(cwd));
        //מחבר את הנתיב הנוכחי ואת הסימן המיוחד
        strncat(cwd, &ch, 1);
       
        // מדפיס את הנתיב הנוכחי והסימן המיוחד ושומר את הפקודה של המתשמש 
        cmdLine = readline(cwd); 
        
        // בודק אם המשתמש הכניס פקודה במידה ולא הוא ממשיך בלולאה
        if(strlen(cmdLine) == 0){
            continue;
        }

        // מבצע פירסור לפקודה מהמשתמש
        info = Parse(cmdLine); 

        // בודק שנשמרו ערכים במשתנה של הפקודה
        if(info == NULL){
            continue;
        }

        
         
        //יוצר תהליך בן 
	    childPid = fork();   
        //במידה זה תהליך בן אז תבצע את הפקודה
        if (childPid == 0)
        {   
            
            /* child code */
           ExecuteCommand(info);
        }
        //אחרת זה התליך אב לכן תמתין עד שהבן אסיים  
        else 
        { 
            /* parent code */
            waitpid(childPid, &status, 0);
        }	 
    } 
}