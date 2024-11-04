#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

// Function to execute a command with redirection if applicable
void execute_command_with_redirection(char *args[]) {
    int i = 0, in_fd = -1, out_fd = -1;
   
    // Loop through the arguments to check for redirection symbols
    while (args[i] != NULL) {
        if (strcmp(args[i], "<") == 0) {  // Input redirection
            in_fd = open(args[i+1], O_RDONLY); // Open the input file
            if (in_fd < 0) {
                perror("Failed to open input file");
                exit(EXIT_FAILURE);
            }
            dup2(in_fd, STDIN_FILENO); // Redirect stdin
            args[i] = NULL; // Remove the redirection part from args
        } else if (strcmp(args[i], ">") == 0) {  // Output redirection
            out_fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644); // Open or create the output file
            if (out_fd < 0) {
                perror("Failed to open output file");
                exit(EXIT_FAILURE);
            }
            dup2(out_fd, STDOUT_FILENO); // Redirect stdout
            args[i] = NULL; // Remove the redirection part from args
        }
        i++;
    }

    // Execute the command
    if (execvp(args[0], args) == -1) {
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }

    // Close the file descriptors if they were used
    if (in_fd != -1) close(in_fd);
    if (out_fd != -1) close(out_fd);
}

// Function to handle pipe execution
void execute_pipe_command(char *cmd1[], char *cmd2[]) {
    int pipe_fd[2]; // File descriptors for the pipe
    pid_t pid1, pid2;

    // Create a pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    // First child: executes cmd1 and writes to pipe
    if ((pid1 = fork()) == 0) {
        dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to pipe write end
        close(pipe_fd[0]); // Close the pipe read end
        close(pipe_fd[1]); // Close the pipe write end
        execvp(cmd1[0], cmd1); // Execute the first command
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }

    // Second child: executes cmd2 and reads from pipe
    if ((pid2 = fork()) == 0) {
        dup2(pipe_fd[0], STDIN_FILENO); // Redirect stdin to pipe read end
        close(pipe_fd[1]); // Close the pipe write end
        close(pipe_fd[0]); // Close the pipe read end
        execvp(cmd2[0], cmd2); // Execute the second command
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }

    // Parent: closes the pipe and waits for children to finish
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    waitpid(pid1, NULL, 0); // Wait for the first child
    waitpid(pid2, NULL, 0); // Wait for the second child
}

int main() {
    char input[1024];
    char *args[64];
    char *pipe_args1[64], *pipe_args2[64];
    int is_pipe = 0;

    while (1) {
        printf("PUCITshell@/home/arif/:- ");
        fgets(input, sizeof(input), stdin); // Get the command input

        // Remove the newline character from the input
        input[strcspn(input, "\n")] = 0;

        // Split input into arguments
        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL) {
            if (strcmp(args[i], "|") == 0) {
                is_pipe = 1;
                args[i] = NULL;
                break;
            }
            i++;
            args[i] = strtok(NULL, " ");
        }

        // Handle pipe commands
        if (is_pipe) {
            int j = 0;
            pipe_args1[j] = args[j]; // First part of the command
            j++;
            pipe_args1[j] = NULL;
           
            int k = 0;
            pipe_args2[k] = strtok(NULL, " "); // Second part of the command
            while (pipe_args2[k] != NULL) {
                k++;
                pipe_args2[k] = strtok(NULL, " ");
            }

            execute_pipe_command(pipe_args1, pipe_args2); // Execute the piped commands
        } else {
            // Fork a new process to execute the command with redirection if applicable
            pid_t pid = fork();
            if (pid == 0) {
                execute_command_with_redirection(args);
            } else {
                wait(NULL); // Wait for the child process to finish
            }
        }

        // Reset pipe flag
        is_pipe = 0;
    }

    return 0;
}
