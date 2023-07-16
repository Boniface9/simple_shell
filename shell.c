#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

void execute_command(char *command) {
    pid_t pid;
    int status;

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        /* Child process */
        char *args[2];
        args[0] = command;
        args[1] = NULL;
        execvp(args[0], args);

        /* execvp only returns if an error occurred */
        perror("execvp");
        exit(1);
    } else {
        /* Parent process */
        do {
            pid_t wpid = waitpid(pid, &status, WUNTRACED);
            if (wpid == -1) {
                perror("waitpid");
                exit(1);
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

int main() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        printf("#cisfun$ ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';  /* Remove trailing newline */

        if (strcmp(command, "exit") == 0) {
            break;
        }

        execute_command(command);
    }

    return 0;
}
