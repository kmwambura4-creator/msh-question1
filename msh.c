#include <stdio.h>// provides printf() and getline()
#include <stdlib.h>// provides free()
#include <sys/types.h>
#include <string.h>// provides strtok()

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

        printf("Command: %s\n", argv[0]);

        for (int i = 0; i < argc; i++)
        {
            printf("Argument %d: %s\n", i, argv[i]);
        }

        
        
    }
    free(line);//realeses the memory allocated for the input line
    return 0;
}