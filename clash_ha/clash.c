#include <stdio.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "plist.h"

// upon starting program:
// display directory

// read input via stdin gets()
// process the input and split it into commands and arguments vith strtok() : seperators: space and tab
// start child processs for each command with fork 
// execute process with corresponding arguments via exec

// if process id foregroud process: wait for processes to teminate (waitpid(2))) and give exit status before taking in new input
// if process is background process aka ends with '&': dont wait, take new promts instantly

// before displaying new prompt: collect zombie processes and give exit status

// compile this file with: gcc -std=c11 -pedantic -D_XOPEN_SOURCE=700 -Wall -Werror -o clash clash.c, execute with ./clash

struct Command{
    char **tokenArray; // array with all the tokens stored seperately
    char *cmdLine;     // the unchanged input from stdin
};
typedef struct Command Command; 


static void die(const char *msg) {                  // this one handles errors and displays a fitting error message
    perror(msg);
    exit(EXIT_FAILURE);
}

int printProcesses(pid_t processId, const char * process){ // this is the print func we need in walkList in the plist file
    printf("[%d] %s \n", processId, process);
    return 0;
}

int cleanProcesses(pid_t processId, const char * process){
    int status;
    if (waitpid(processId, &status, WUNTRACED | WCONTINUED | WNOHANG)!= 0){  // if not 0, the process has already terminated so we remove it 
        removeElement(processId, NULL, 0);  
    }
    return 0;
}
    

static void getDir(){
    char cwd[4096];                                 // max filesystem pathlength is usually 4096 say smart people on the internet
    if (getcwd(cwd, sizeof(cwd)) != 0){
        printf(">>> %s\n", cwd);
    }
    else{
        die("getcwd() failed");
    }
}

static Command processInput(){
    char ** tokenArray = malloc(1);                 // an array containing pointers to all of the tokens
    int tokenCounter = 0;
    static char newInput[1340];                     // create Array with max length of input (taken from aufgabenstellung)
                                                    // must be static so the token ptrs in tokenArray outlive the scope of the function

    fgets(newInput, 1340 ,stdin);                   // get input fgets(array we want to write to, how many chars, source)
    if(strlen(newInput)> 1337){                     // check for correct input length
        printf("input longer than 1337 chars");
        Command errorCmd = {NULL,NULL};
        return errorCmd;
    }

    if(feof(stdin)!= 0){                            // if end of file reached, end program
        walkList(cleanProcesses);
        exit(0);
        }
    const char delimiters[] = {' ','\t', 0};        // setzen der zeichen an denen getrennt wird
    char *token = strtok(newInput, delimiters);     // beginn des auftrennens des strings an den delimitern 

    tokenArray = realloc(tokenArray, (tokenCounter + 1) * sizeof(char*));       // Token array um die länge des neuen tokens erhöhen (da es ein char pointer array is gilt sizeof array = #pointer * pointergröße
    tokenArray[tokenCounter] = token;                                           // Token anfügen ans Array

    do{
        tokenCounter += 1;
        token = strtok(NULL, delimiters);                                           // einen token weitergehen, da wir strtok mit null aufgerufen haben

        tokenArray = realloc(tokenArray, (tokenCounter + 1) * sizeof(char*));       // Token array um die länge des neuen tokens erhöhen (da es ein char pointer array is gilt sizeof array = #pointer * pointergröße
        tokenArray[tokenCounter] = token;                                           // Token anfügen ans Array

    }while (token != NULL);                            

    Command newCmd;
    newCmd.tokenArray = tokenArray;
    newCmd.cmdLine = newInput;
    fflush(stdin);
    return newCmd;
}


static int executeCmd(Command execCmd){                 // tokenarray[0] is command, rest of tokenarray are arguments
    int status;
    int background = 0;
    int i;

    char **tokenArray = execCmd.tokenArray;
    char *rawInput = execCmd.cmdLine;

    //specialFunctions are "cd", "&", "jobs"
    
    // find out if it is going to be a subprocess
    for(i =0; tokenArray[i] != NULL; i++){continue;};    // for some reason this is allowed (we use this to get the length of tokenArray)

    // find out if a special command is used

    if(strcmp(tokenArray[i-1], "&") == 0){              // if last token is '&' then we got ourselves a good old background process
        background = 1;
        tokenArray[i-1] = NULL;                         // delete background flag to not cause wrong output
    }
    else if(!strcmp(tokenArray[0], "cd")){
        // todo: change directory
        const char* onlyCommand = *tokenArray + 1;      // kinda skip the command
        int chdirReturn  = chdir(onlyCommand);
        if(chdirReturn == -1){
            die("chdir failed");
        }
        return 0; 
    }

    else if(!strcmp(tokenArray[0], "jobs")){
        walkList(printProcesses); 
        return 0;
    }


    // if we haven't returned already we execute a normal prompt

    pid_t child = fork(); // create child proces. returns child's pid

    // idea: when we fork, we have two frolocking processes who both go through the if thingies below independently, 
    // thats why the child execs and terminates invisibly
    // and the status is always set, because at some point in time, the parent will go through the if clauses and enter the else part.

    if (child == -1){
        die("something went wrong with fork()");
    }
    else if(child == 0){ // current process is child process
        if(background){
            int execution = execvp(tokenArray[0], tokenArray);      // execv doesn't write into the console -> background process
            if(execution == -1){ 
                die("something went wrong with execvp() in a background process");
            }
        else{
            int execution = execvp(tokenArray[0], tokenArray);      // execvp(command aka token 0, allesauchcommand aka alle tokens), exec returns -1 if it failed, else it doesnt return at all
            if(execution == -1){
                die("something went wrong with execvp in the foreground");
            }
        printf("child executed");
        }
        exit(EXIT_SUCCESS);                     // void exit(int status) EXIT_SUCCESS = 0, EXIT_FAILURE != 0, usually 1         
                                                // exit frees all resources the process allocated, process becomes zombie
        }
    }

    else{        // we are parent process
        insertElement(child, rawInput);                         // insert child process into process list from plist
        
        if(!background){
            waitpid(child, &status, WUNTRACED | WCONTINUED);    // waitpid(processID (pid_t), adress of status with which child has exited (integer), misc options from stack overflow)
            removeElement(child, NULL, 0);                      // we waited for child process to be done, now it gets removed from active processes list
            printf("Exitstatus [%s] %d", rawInput, status);      
        }
        else{ return status;}                                   // waitpid will write in the status location the according status with the process has terminated
    }

    return status;
}


int main(int argc, char **argv)
{
    while(1){
        getDir();
        Command input = processInput();
        executeCmd(input);
        walkList(cleanProcesses);
        free(input.tokenArray);
        free(input.cmdLine);
    }
    return 0;
}