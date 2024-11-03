#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>

void run_shell() {
    char command[256];
    char *args[10];

    while (1) {
        // Display prompt with current directory
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        printf("PUCITshell@%s:- ", cwd);
        
        // Read input
        if (fgets(command, sizeof(command), stdin) == NULL) break;
        
        // Tokenize input
        int i = 0;
        args[i] = strtok(command, " \n");
        while (args[i] != NULL) {
            args[++i] = strtok(NULL, " \n");
        }
        
        if (args[0] == NULL) continue;  // Skip if empty command

        // Fork and execute
        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            perror("execvp failed");  // Only reached if execvp fails
            exit(1);
        } else if (pid > 0) {
            wait(NULL);  // Wait for the child process
        } else {
            perror("fork failed");
        }
    }
}

int main() {
    run_shell();
    return 0;
}
