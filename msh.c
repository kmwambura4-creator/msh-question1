#include <stdio.h>// provides printf() and getline()
#include <stdlib.h>// provides free()
#include <sys/types.h>

int main()
{
    char *line=NULL;//creats a poniter that will hold the the inputline
    size_t capacity=0;// keeps track of the memory available for the input
    ssize_t length; //stores the number of characters read by getline

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
        printf("%s", line);
        
    }
    free(line);//realeses the memory allocated for the input line
    return 0;
}