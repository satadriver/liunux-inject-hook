#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        /* 子进程 */
        ptrace(PT_TRACE_ME, 0, 0, 0);
        execl("/bin/ls", "ls", (char *)NULL);
        perror("execl");
        exit(1);
    } else {
        /* 父进程 */
        wait(&status);
        
        if (WIFSTOPPED(status)) {
            printf("Child stopped, now tracing...\n");
            
            /* 继续执行 */
            ptrace(PT_CONTINUE, pid, (void *)1, 0);
            
            wait(&status);
            if (WIFEXITED(status)) {
                printf("Child exited with status %d\n", WEXITSTATUS(status));
            }
        }
    }

    return 0;
}