include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARS 100

struct var {
    char *str;   // name=value string
    int global;  // 1 for global variable, 0 for local variable
};

struct var variables[MAX_VARS];
int var_count = 0;

// Function to set a variable
void set_variable(char *name, char *value, int global) {
    if (var_count < MAX_VARS) {
        char *entry = malloc(strlen(name) + strlen(value) + 2);
        sprintf(entry, "%s=%s", name, value);

        // Check if variable already exists
        for (int i = 0; i < var_count; i++) {
            if (strncmp(variables[i].str, name, strlen(name)) == 0) {
                free(variables[i].str);
                variables[i].str = entry;
                variables[i].global = global;
                return;
            }
        }

        // Add new variable
        variables[var_count].str = entry;
        variables[var_count].global = global;
        var_count++;
    } else {
        printf("Variable limit reached.\n");
    }
}

// Function to get a variable
char *get_variable(char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strncmp(variables[i].str, name, strlen(name)) == 0) {
            return strchr(variables[i].str, '=') + 1; // Return value
        }
    }
    return NULL; // Not found
}

// Function to list all variables
void list_variables() {
    for (int i = 0; i < var_count; i++) {
        printf("%s\n", variables[i].str);
    }
}

// Function to free memory
void free_variables() {
    for (int i = 0; i < var_count; i++) {
        free(variables[i].str);
    }
}

// Main shell loop
int main() {
    char command[256];
    char name[128];
    char value[128];
    
    while (1) {
        printf("shell> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // Remove newline character

        if (strncmp(command, "set ", 4) == 0) {
            sscanf(command + 4, "%s %s", name, value);
            set_variable(name, value, 1); // Assume global for simplicity
        } else if (strncmp(command, "get ", 4) == 0) {
            sscanf(command + 4, "%s", name);
            char *val = get_variable(name);
            if (val) {
                printf("%s=%s\n", name, val);
            } else {
                printf("Variable not found.\n");
            }
        } else if (strcmp(command, "list") == 0) {
            list_variables();
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else {
            printf("Unknown command.\n");
        }
    }

    free_variables();
    return 0;
}
