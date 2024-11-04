#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

// Signal handler to reap zombie processes
void handle_sigchld(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

// Function to run the shell
void run_shell() {
    char input[1024];
    char *args[64];
    pid_t pid;
    int background = 0;

    // Register signal handler to avoid zombie processes
    signal(SIGCHLD, handle_sigchld);

    while (1) {
        background = 0;  // Reset background flag
        printf("PUCITshell@/home/arif/:- ");
        fgets(input, sizeof(input), stdin);  // Get user input

        // Remove newline character from the input
        input[strcspn(input, "\n")] = 0;

        // Split input into arguments
        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }

        // Check if the command should run in the background
        if (i > 0 && strcmp(args[i - 1], "&") == 0) {
            background = 1;
            args[i - 1] = NULL;  // Remove '&' from arguments
        }

        // Fork a new process
        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // In child process, execute the command
            if (execvp(args[0], args) == -1) {
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
        } else {
            // In parent process
            if (!background) {
                // Wait for the child to finish if not running in the background
                waitpid(pid, NULL, 0);
            } else {
                // Print the process ID for background processes
                printf("[1] %d\n", pid);
            }
        }
    }
}

int main() {
    run_shell();
    return 0;
}

