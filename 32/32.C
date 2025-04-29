#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024
#define MAX_CMD_LEN 256
#define MAX_CMDS 1000

// Structure to store a command and its frequency
typedef struct {
    char command[MAX_CMD_LEN];
    int count;
} CommandFreq;

// Comparison function for qsort - sorts in descending order based on count
int compareCommands(const void *a, const void *b) {
    const CommandFreq *cmdA = (const CommandFreq *)a;
    const CommandFreq *cmdB = (const CommandFreq *)b;
    
    // Sort by count (descending)
    return cmdB->count - cmdA->count;
}

// Function to extract command name from a history line
// Typically history lines are in format "123 command arguments"
void extractCommand(char *line, char *command) {
    // Skip the history number
    char *ptr = line;
    while (*ptr && (*ptr == ' ' || isdigit(*ptr))) {
        ptr++;
    }
    
    // Extract the command (first word after spaces/numbers)
    int i = 0;
    while (*ptr && *ptr != ' ' && i < MAX_CMD_LEN - 1) {
        command[i++] = *ptr++;
    }
    command[i] = '\0';
}

// Function to check if a command already exists in our array
// Returns the index if found, -1 otherwise
int findCommand(CommandFreq *cmdList, int cmdCount, char *command) {
    for (int i = 0; i < cmdCount; i++) {
        if (strcmp(cmdList[i].command, command) == 0) {
            return i;
        }
    }
    return -1;
}

int main() {
    FILE *pipe;
    char line[MAX_LINE_LEN];
    char command[MAX_CMD_LEN];
    CommandFreq cmdList[MAX_CMDS];
    int cmdCount = 0;
    
    // Open pipe to history command
    pipe = popen("history", "r");
    if (pipe == NULL) {
        printf("Error executing history command\n");
        return 1;
    }
    
    // Read each line from history
    while (fgets(line, MAX_LINE_LEN, pipe) != NULL && cmdCount < MAX_CMDS) {
        // Remove newline character
        line[strcspn(line, "\n")] = '\0';
        
        // Extract command from history line
        extractCommand(line, command);
        
        // Skip empty commands
        if (strlen(command) == 0) {
            continue;
        }
        
        // Check if command exists in our list
        int index = findCommand(cmdList, cmdCount, command);
        
        if (index != -1) {
            // Command found, increment count
            cmdList[index].count++;
        } else {
            // New command, add to list
            strcpy(cmdList[cmdCount].command, command);
            cmdList[cmdCount].count = 1;
            cmdCount++;
        }
    }
    
    // Close pipe
    pclose(pipe);
    
    // Sort commands by frequency (descending)
    qsort(cmdList, cmdCount, sizeof(CommandFreq), compareCommands);
    
    // Display top 5 commands (or fewer if less than 5 commands were found)
    printf("Top %d most used commands:\n", (cmdCount < 5) ? cmdCount : 5);
    for (int i = 0; i < cmdCount && i < 5; i++) {
        printf("%d. %s: %d times\n", i + 1, cmdList[i].command, cmdList[i].count);
    }
    
    return 0;
}