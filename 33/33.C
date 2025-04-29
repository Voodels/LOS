#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// Function to check if a file is executable by the current user
int isExecutable(const char *filename) {
    // Use access() function to check if the file can be executed
    // X_OK flag checks for execute permission
    return access(filename, X_OK) == 0;
}

// Function to remove execute permissions from a file
int removeExecutePermissions(const char *filename) {
    struct stat st;
    mode_t newMode;
    
    // Get current file permissions
    if (stat(filename, &st) != 0) {
        perror("Error getting file status");
        return 0;
    }
    
    // Calculate new permissions by removing execute bits
    // We use bitwise AND with complement of execute bits:
    // ~(S_IXUSR | S_IXGRP | S_IXOTH) removes execute for user, group, others
    newMode = st.st_mode & ~(S_IXUSR | S_IXGRP | S_IXOTH);
    
    // Apply new permissions
    if (chmod(filename, newMode) != 0) {
        perror("Error changing permissions");
        return 0;
    }
    
    return 1;
}

int main(int argc, char *argv[]) {
    // Check if a filename was provided
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    // Check if file exists
    if (access(argv[1], F_OK) != 0) {
        printf("Error: File '%s' does not exist\n", argv[1]);
        return 1;
    }
    
    // Check if file is executable
    if (isExecutable(argv[1])) {
        printf("File '%s' is executable.\n", argv[1]);
        
        // Remove execute permissions
        printf("Removing execute permissions...\n");
        if (removeExecutePermissions(argv[1])) {
            printf("Execute permissions removed successfully.\n");
        } else {
            printf("Failed to remove execute permissions.\n");
        }
    } else {
        printf("File '%s' is not executable.\n", argv[1]);
    }
    
    return 0;
}