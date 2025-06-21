#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#ifndef CHILD_EXIT_STATUS
#define CHILD_EXIT_STATUS 42
#endif

#define EXPECTED_CODE 42

void child_routine(pid_t pid)
{
    printf("This is the child routine... Recieved PID is: %d\n", pid);
    while (1)
        continue;
};

void kill_and_get_children(pid_t *pid)
{
    int i;
    int status;
    printf("Parent: I'm the murderous parent.\n");
    while (i < 3)
    {
        kill(pid[i], SIGKILL);
        i++;
    };
    i = 0;
    while (i < 3)
    {
        waitpid(pid[i], &status, 0);
        if (WIFEXITED(status))
        {
            printf("Parent: Child [%d] terminated normally.\n", pid[i]);
        }
        else if (WIFSIGNALED(status))
        {
            printf("Parent: Child [%d] was interrupted.\n", pid[i]);
            if (WTERMSIG(status) == SIGTERM)
            {
                printf("Parent: Child [%d] got the %d signal, SIGTERM\n", pid[i], WTERMSIG(status));
            }
            if (WTERMSIG(status) == SIGKILL)
            {
                printf("Parent: Child [%d] got the %d signal, SIGKILL\n", pid[i], WTERMSIG(status));
            }
            printf("Parent: Child [%d] terminated normally.\n", pid[i]);
        }
        i++;
    }
};

int main()
{
    pid_t pid[3];
    int i = 0;

    while (i < 3)
    {
        pid[i] = fork();
        if (pid[i] == -1)
        {
            return EXIT_FAILURE;
        }
        if (pid[i] == 0)
        {
            child_routine(pid[i]);
        }
        else if (pid[i] > 0)
        {
            printf("Parent: Child #%d created with pid = %d\n", i, pid[i]);
        }
        usleep(1000); // Delay next child's creation
        i++;
    };
    kill_and_get_children(pid);
    return (EXIT_SUCCESS);
}