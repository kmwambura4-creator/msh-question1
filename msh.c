#include <stdio.h>// provides printf() and getline()
#include <stdlib.h>// provides free()
#include <sys/types.h>
#include <string.h>// provides strtok()
#include <unistd.h>
#include <sys/wait.h>//produces waitpid()
#include <limits.h>
#include <fcntl.h>//provides open
#include <signal.h>//provide signal(),SIGNT,SIG_IGN SIG_DFL

int main()
{
    char *line=NULL;//creats a poniter that will hold the the inputline
    size_t capacity=0;// keeps track of the memory available for the input
    ssize_t length; //stores the number of characters read by getline

    char *argv[51];//argv stores pointers to individual argument
    int argc;//counts number of arguments
    int last_status=0;//variable stores the exit status of previous command
    if (signal(SIGINT, SIG_IGN) == SIG_ERR)//tells parent process to ignore SIGINT which is uesd to terminate when crtl+c is pressed
     {
       perror("signal");
       return 1;
     }
    while (1)//creates an infinite loop so the shell continues to accept commands
    {
      int status;
      while(waitpid(-1,&status,WNOHANG)>0)//prevents zombie process
      {/* compleated background process*/}
        printf("msh[%d]>",last_status);//shows the value stored in last_status
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
        char *output_file=NULL;

        for(int i=0; i<argc; i++)
        {
          if(strcmp(argv[i],">")==0)
          {
            if(i+1<argc)
            {
              output_file =argv[i+1];
              argv[i]=NULL;
              argc=i;
            }
            break;
          }
        }
        int background=0;

        if(argc > 0 && strcmp(argv[argc-1],"&")==0)//checks if last argument is &
        {
          background=1;
          argv[argc-1]=NULL;//removes & from our execvp
          argc--;//decreases the argument count
        }

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
          if (signal(SIGINT, SIG_DFL) == SIG_ERR)//SIG_DEF makes the child process to follow the deafult action for SIGINT
            {
              perror("signal");
              exit(1);
            }
          if(output_file !=NULL)
          {
            int fd=open(output_file, O_WRONLY| O_CREAT| O_TRUNC,0644);//creates the file and file permission
            if(fd ==-1)
            {
              perror("open");
              exit(1);
            }
            if(dup2(fd,STDOUT_FILENO)==-1)//redirection 
            {
              perror("dup2");
              close(fd);
              exit(1);
            }
            close(fd);//closed because standard output now has its own reference to opened file
          }
          execvp(argv[0], argv);//execvp replaces the child process

          perror("execvp");
          exit(127);
         }

         else//parent process
         {
          int status;
          if (background)
         {
          printf("Background process pid %d\n",pid);
         }
         else
          {
          if(waitpid(pid, &status, 0)==-1)
          {
            perror("waitpid");
            last_status=1;
          }
          else if(WIFEXITED(status))//checks if child process terminated normally
          {
            last_status=WEXITSTATUS(status);//extracts the actual exit status returned by the child
          }
          else if(WIFSIGNALED(status))
          {
            last_status=128+ WTERMSIG(status);//used to obtain the signal number
          }
          }
         }
        
    }
    free(line);//realeses the memory allocated for the input line
    return 0;
}