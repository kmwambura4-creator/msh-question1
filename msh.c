#include <stdio.h>// provides printf() and getline()
#include <stdlib.h>// provides free()
#include <sys/types.h>
#include <string.h>// provides strtok()
#include <unistd.h>
#include <sys/wait.h>//produces waitpid()
#include <limits.h>//

int main()
{
    char *line=NULL;//creats a poniter that will hold the the inputline
    size_t capacity=0;// keeps track of the memory available for the input
    ssize_t length; //stores the number of characters read by getline

    char *argv[51];//argv stores pointers to individual argument
    int argc;//counts number of arguments

    while (1)//creates an infinite loop so the shell continues to accept commands
    {
        printf("msh> ");
        length =getline(&line, &capacity ,stdin);
        /*&line is the address of the pointer where the input will be stored
        &capacity is address of the variable tracking allocated memory
        stdin reads input from keyboard
        */

        if (length==-1)//exit on end-of-file
        {
            break;
        }

         argc = 0;

        char *token = strtok(line, " \t\n");//finds the first word

        while (token != NULL && argc < 50)//ensures it takes 49 arguments 
        {
            argv[argc] = token;
            argc++;

            token = strtok(NULL, " \t\n");//gets the rest of the world
        }

        argv[argc] = NULL;//to ensure the end of the arguments

        if (argc == 0)//ensures it ignores blank lines
        {
            continue;
        }
              if (strcmp(argv[0], "cd") == 0)//used to compare two stings and return 0 when equal and add the cd
                 {
                   if (argv[1] == NULL)//checks whether argument was provided
                    {
                     char *home = getenv("HOME");

                     if (home == NULL)
                     {
                          fprintf(stderr, "HOME not set\n");
                     }
                    else if (chdir(home) != 0)
                     {
                       perror("cd");
                     }
                    }
                     else if (chdir(argv[1]) != 0)//changes the directory of the cirrent process
                     {
                       perror("cd");
                     }

                    continue;//this ensures our cd does not accidentary reach our fork
                 }   
                 if (strcmp(argv[0], "help") == 0)//add help command
                  {
                   printf("Welcome to msh!\n");
                   printf("Available commands:\n");
                   printf("cd [dir] - Change directory\n");
                   printf("help     - Display this help message\n");
                   printf("exit     - Exit the shell\n");

                   continue;
                  } 
                  if (strcmp(argv[0], "exit") == 0)//checks whether exit if inpued then the block code will be executed exiting the while loop to the free() line
                   {
                      break;
                   }
 
       pid_t pid = fork();

        if (pid < 0)//if the fork fails
         {
           perror("fork");
         }

        else if (pid == 0)//the child process
         {
          execvp(argv[0], argv);//execvp replaces the child process

          perror("execvp");
          exit(127);
         }

         else
         {
          int status;
          waitpid(pid, &status, 0);//the parent waits for the child process to finish
          }
        
    }
    free(line);//realeses the memory allocated for the input line
    return 0;
}