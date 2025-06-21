#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void child_routine(int *number)
{
    printf("We are in the child routine\n");
    sleep(1);
    printf("Number from child routine -> %d\n", *number);
};
void parent_routine(int *number, int *status)
{
    *number = 8;
    // Without waiting, the parent process is going to terminate without waiting for the child
    wait(status);
    printf("We are in the parent routine\n");
    printf("Number from parent routine -> %d\n", *number);
}
int main()
{
    pid_t pid;
    int number = 4;
    int status;
    printf("Forking...\n");
    pid = fork();
    number = 5;
    if (pid < 0)
    {
        perror("Error while forking\n");

        exit(1);
    };
    if (pid == 0)
    {
        child_routine(&number);
    }
    else
    {
        parent_routine(&number, &status);
    }
}