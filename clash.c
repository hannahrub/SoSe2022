#include <stdio.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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



static void die(const char *msg) {                  // this one handles errors and displays a fitting error message
    perror(msg);
    exit(EXIT_FAILURE);
}

static void getDir(){
    char cwd[4096];                                 // max filesystem pathlength is  usually 4096
    if (getcwd(cwd, sizeof(cwd)) != 0){
        printf("current working dir %s\n", cwd);
    }
    else{
        die("getcwd() failed");
    }
}

static char **processInput(){
    char ** tokenArray = malloc(1);                 // an array containing pointers to all of the tokens
    int tokenCounter = 0;
    static char newInput[1337];                     // create Array with max length of input (taken from aufgabenstellung)
                                                    // must be static so the token ptrs in tokenArray outlive the scope of the function

    fgets(newInput, 1337 ,stdin);                   // get input fgets(array we want to write to, how many chars, source)
    const char delimiters[] = {' ','\t', 0};        // setzen der zeichen an denen getrennt wird
    char *token = strtok(newInput, delimiters);     // beginn des auftrennens des strings an den delimitern 

    do{
        tokenArray = realloc(tokenArray, (tokenCounter + 1) * sizeof(char*));       // Token array um die länge des neuen tokens erhöhen (da es ein char pointer array is gilt sizeof array = #pointer * pointergröße
        tokenArray[tokenCounter] = token;                                           // Token anfügen ans Array

        tokenCounter += 1;
        token = strtok(NULL, delimiters);                                           // einen token weitergehen, da wir strtok mit null aufgerufen haben
    }while (token != NULL);                             //todo: does this have to be a do while func? maybe, because all token thingies end with 0???????????????

    return tokenArray;
}


static int executeCmd(char **tokenArray){         // tokenarray[0] is command, rest of tokenarray are arguments
    int status;
    pid_t child = fork();
    printf("we should be executing rn \n");

    // idea: when we fork, we have two frolocking processes who both go through the if thingies below independently, 
    // thats why the child execs and terminates invisibly
    // and the status is always set, because at some point in time, the parent will go through the if clauses and enter the else part.

    if (child == -1){
        // awesome error handling here
        printf("something is foul in the state of denmark \n");
    }
    else if(child == 0){             // current process is child process
        printf("token array %s \n", tokenArray[0]);
        execvp(tokenArray[0], tokenArray);    // execvp(command aka token 0, allesauchcommand aka alle tokens)
        exit(EXIT_SUCCESS);                   // void exit(int status) EXIT_SUCCESS = 0, EXIT_FAILURE != 0, usually 1         
    }                                         // exit frees all resources the process allocated, process becomes zombie

    else{        // we are parent process
       waitpid(child, &status, WUNTRACED | WCONTINUED);     // waitpid(processID (pid_t), adress of status with which child has exited (integer), misc options from stack overflow)
    }                                                       // waitpid will write in the status location the according status with the process has terminated
    return status;
}


int main(int argc, char **argv)
{
    while(1){
        getDir();
        char **input = processInput();
        printf("token[0] %s \n", input[0]);
        printf("token[1] %s \n", input[1]);
        executeCmd(input);

    }
    return 0;
}