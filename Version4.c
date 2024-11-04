#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_HISTORY 10
#define MAX_COMMAND_LENGTH 1024

// Function to print the command history
void print_history(char history[MAX_HISTORY][MAX_COMMAND_LENGTH], int history_count) {
    int start = history_count >= MAX_HISTORY ? history_count - MAX_HISTORY : 0;
    for (int i = start; i < history_count; i++) {
        printf("%d: %s\n", i + 1, history[i % MAX_HISTORY]);
    }
}

// Function to execute a command
void execute_command(char *command) {
    char *args[64];
    int i = 0;

    // Split command into arguments
    args[i] = strtok(command, " ");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " ");
    }

    // Fork a new process
    pid_t pid = fork();
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
        // In parent process, wait for the child to finish
        waitpid(pid, NULL, 0);
    }
}

// Function to handle a !number command
void handle_bang_command(char *input, char history[MAX_HISTORY][MAX_COMMAND_LENGTH], int history_count) {
    if (input[0] == '!') {
        int command_num;

        // Handle the case for !-1 (repeat the last command)
        if (strcmp(input, "!-1") == 0) {
            command_num = history_count - 1;
        } else {
            // Convert the rest of the input to an integer for !number
            command_num = atoi(&input[1]) - 1;
        }

        // Ensure the command number is valid
        if (command_num < 0 || command_num >= history_count || command_num < history_count - MAX_HISTORY) {
            printf("Invalid command number!\n");
        } else {
            // Repeat the command from history
            printf("Repeating command: %s\n", history[command_num % MAX_HISTORY]);
            execute_command(history[command_num % MAX_HISTORY]);
        }
    }
}

// Main function to run the shell
int main() {
    char input[MAX_COMMAND_LENGTH];
    char history[MAX_HISTORY][MAX_COMMAND_LENGTH];
    int history_count = 0;

    while (1) {
        printf("PUCITshell@/home/arif/:- ");
        fgets(input, sizeof(input), stdin);

        // Remove newline character from input
        input[strcspn(input, "\n")] = 0;

        // Check if the user wants to repeat a command with !number
        if (input[0] == '!') {
            handle_bang_command(input, history, history_count);
        } else if (strcmp(input, "history") == 0) {
            // Print the command history
            print_history(history, history_count);
        } else {
            // Store the command in history
            strcpy(history[history_count % MAX_HISTORY], input);
            history_count++;

            // Execute the command
            execute_command(input);
        }
    }

    return 0;
}
