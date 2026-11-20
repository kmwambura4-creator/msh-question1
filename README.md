msh - Minimal Unix Shell
Name: kelvin mwangi
Student ID: SCT212-0042/2024

How to Build and Run
make
./msh
Features Implemented
 Task 1: Prompt, getline(), clean exit on Ctrl-D without memory leaks (Complete)
 Task 2: Tokenize arguments on spaces/tabs, ignore blank lines, NULL-terminate argv (Complete)
 Task 3: External commands using fork(), execvp(), waitpid(), and error reporting (Complete)
 Task 4: Built-ins cd [dir] (defaults to $HOME), help, and exit (Complete)
 Task 5: Exit status in prompt msh[N]>, handling normal exit and signals (128 + sig) (Complete)
 Task 6: Background execution (&), printing child PID, and reaping zombies with WNOHANG (Complete)
 Task 7: Output redirection (>) in child process only (open, dup2, close, mode 0644) (Complete)
 Task 8: Ctrl-C (SIGINT) interrupts foreground jobs without terminating the shell (Complete)
Design Explanation
msh executes external commands using the standard Unix fork() / execvp() / waitpid() cycle. After reading a line with getline() and splitting it into a NULL-terminated argv array with strtok(), the shell calls fork() to duplicate itself into a child process. In the child process, SIGINT is restored to its default handler, any requested > output redirection is applied via open() and dup2(), and execvp() replaces the child's process image with the requested program. If execvp() fails, the child prints an error via perror() and calls _exit(127). Meanwhile, the parent process waits for foreground jobs with waitpid() and extracts the exit code using WIFEXITED/WEXITSTATUS or 128 + WTERMSIG if terminated by a signal.

cd and exit must be implemented as built-ins rather than external programs because external programs run in a child process. A child process has its own working directory and process lifecycle: calling chdir() or exit() inside a child would only change or terminate the child, leaving the parent shell's directory unchanged and the shell still running.

To prevent zombie processes from background jobs (&), the shell calls waitpid(-1, &status, WNOHANG) in a loop before every prompt, reaping any terminated background children without blocking user input.

Known Limitations
Does not support input redirection (<), append redirection (>>), or pipes (|).
Does not handle quoted arguments containing spaces (e.g., echo "hello world").
Sources and AI-Tool Declaration
Linux man pages (fork(2), execvp(3), waitpid(2), sigaction(2), open(2), dup2(2)).
AI assistance (Gemini) was used to help design, review, and document the shell implementation.
