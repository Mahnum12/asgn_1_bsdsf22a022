#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64
#define MAX_JOBS 10

typedef struct {
    pid_t pid;
    char command[MAX_COMMAND_LENGTH];
    int in_use;
} Job;

Job jobs[MAX_JOBS]; // Array to store background jobs
int job_count = 0;

// Function to display the list of built-in commands
void show_help() {
    printf("Built-in commands:\n");
    printf("  cd <directory>: Change the current directory\n");
    printf("  exit: Exit the shell\n");
    printf("  jobs: List background jobs\n");
    printf("  kill <job_number>: Kill the background job with the given number\n");
    printf("  help: Display this help message\n");
}

// Function to add a background job
void add_job(pid_t pid, char *command) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (!jobs[i].in_use) {
            jobs[i].pid = pid;
            strcpy(jobs[i].command, command);
            jobs[i].in_use = 1;
            job_count++;
            return;
        }
    }
    printf("Job list is full.\n");
}

// Function to list background jobs
void list_jobs() {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].in_use) {
            printf("[%d] PID: %d, Command: %s\n", i + 1, jobs[i].pid, jobs[i].command);
        }
    }
}

// Function to kill a background job
void kill_job(int job_number) {
    if (job_number < 1 || job_number > MAX_JOBS || !jobs[job_number - 1].in_use) {
        printf("Invalid job number.\n");
        return;
    }

    pid_t pid = jobs[job_number - 1].pid;
    if (kill(pid, SIGKILL) == 0) {
        printf("Job [%d] with PID %d killed.\n", job_number, pid);
        jobs[job_number - 1].in_use = 0;
        job_count--;
    } else {
        perror("kill failed");
    }
}

// Function to change the current working directory
void change_directory(char *path) {
    if (chdir(path) == -1) {
        perror("chdir failed");
    }
}

// Function to execute a command (external or built-in)
void execute_command(char *input) {
    char *args[MAX_ARGS];
    int i = 0;

    // Split command into arguments
    args[i] = strtok(input, " ");
    while (args[i] != NULL && i < MAX_ARGS - 1) {
        i++;
        args[i] = strtok(NULL, " ");
    }

    // Handle built-in commands
    if (strcmp(args[0], "cd") == 0) {
        if (args[1]) {
            change_directory(args[1]);
        } else {
            printf("cd: expected argument\n");
        }
        return;
    } else if (strcmp(args[0], "exit") == 0) {
        exit(0);
    } else if (strcmp(args[0], "jobs") == 0) {
        list_jobs();
        return;
    } else if (strcmp(args[0], "kill") == 0) {
        if (args[1]) {
            int job_number = atoi(args[1]);
            kill_job(job_number);
        } else {
            printf("kill: expected job number\n");
        }
        return;
    } else if (strcmp(args[0], "help") == 0) {
        show_help();
        return;
    }

    // External command execution
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
    } else if (pid == 0) {
        // In child process, execute the command
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
        }
        exit(EXIT_FAILURE);
    } else {
        // In parent process, check for background job
        int background = 0;
        if (i > 1 && strcmp(args[i - 1], "&") == 0) {
            background = 1;
            args[i - 1] = NULL; // Remove '&' from arguments
        }

        if (background) {
            add_job(pid, input);
        } else {
            waitpid(pid, NULL, 0); // Wait for the child process if not background
        }
    }
}

// Main shell loop
int main() {
    char input[MAX_COMMAND_LENGTH];

    while (1) {
        printf("PUCITshell@/home/user/:- ");
        fgets(input, sizeof(input), stdin);

        // Remove newline character from input
        input[strcspn(input, "\n")] = 0;

        // Skip empty input
        if (strlen(input) == 0) {
            continue;
        }

        // Execute the command
        execute_command(input);
    }

    return 0;
}
