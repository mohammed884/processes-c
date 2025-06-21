#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifndef CHILD_EXIT_STATUS
#define CHILD_EXIT_STATUS 42
#endif

#define EXPECTED_CODE 42

void child_routine(pid_t pid)
{
    printf("This is the child routine... Recieved PID is: %d\n", pid);
    printf("Exiting with the exit code of %d", CHILD_EXIT_STATUS);
    exit(CHILD_EXIT_STATUS);
};

void parent_routine(pid_t pid)
{
    int status;
    printf("This is the parent routine... Recieved PID is: %d\n", pid);
    waitpid(pid, &status, 0);
    /*
    WIFEXITED = child exited normally
    specifically by calling exit() or _exit() or returning from main().
    */
    /*
    WEXITSTATUS macro returns the exit code specified by the child, and if it didn't exit normally,
    then the code won't match the intended
    */
    if (WIFEXITED(status))
    {
        printf("Parent: My Child's exit code is %d\n", status);
        if (WEXITSTATUS(status) == CHILD_EXIT_STATUS)
        {
            printf("Expected child exit code\n");
        }
        else
        {
            printf("Not an Expected child exit code\n");
        }
    }
};

int main()
{
    pid_t pid;

    pid = fork();

    if (pid == -1)
    {
        return EXIT_FAILURE;
    }
    if (pid == 0)
    {
        child_routine(pid);
    }
    else
    {
        parent_routine(pid);
    }
}