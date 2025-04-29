#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define MAX_WORDS 10000

// Structure to store a word and its frequency
typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordFreq;

// Function to convert a word to lowercase
void toLowerCase(char *word) {
    for (int i = 0; word[i]; i++) {
        word[i] = tolower(word[i]);
    }
}

// Function to check if a word already exists in our array
// Returns the index if found, -1 otherwise
int findWord(WordFreq *wordList, int wordCount, char *word) {
    for (int i = 0; i < wordCount; i++) {
        if (strcmp(wordList[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char *argv[]) {
    FILE *file;
    char word[MAX_WORD_LEN];
    WordFreq wordList[MAX_WORDS];
    int wordCount = 0;
    char ch;
    int i = 0;
    
    // Check if filename is provided
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    // Open the file
    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file %s\n", argv[1]);
        return 1;
    }
    
    // Read the file character by character
    while ((ch = fgetc(file)) != EOF && wordCount < MAX_WORDS) {
        // If character is alphanumeric, add it to current word
        if (isalnum(ch) || ch == '\'') {
            if (i < MAX_WORD_LEN - 1) {
                word[i++] = ch;
            }
        } 
        // If it's a delimiter, process the current word
        else if (i > 0) {
            word[i] = '\0';  // Null-terminate the string
            toLowerCase(word);  // Convert to lowercase for case-insensitive matching
            
            // Check if word exists in our list
            int index = findWord(wordList, wordCount, word);
            
            if (index != -1) {
                // Word found, increment count
                wordList[index].count++;
            } else {
                // New word, add to list
                strcpy(wordList[wordCount].word, word);
                wordList[wordCount].count = 1;
                wordCount++;
            }
            
            // Reset for next word
            i = 0;
        }
    }
    
    // Process the last word if file doesn't end with a delimiter
    if (i > 0) {
        word[i] = '\0';
        toLowerCase(word);
        
        int index = findWord(wordList, wordCount, word);
        
        if (index != -1) {
            wordList[index].count++;
        } else {
            strcpy(wordList[wordCount].word, word);
            wordList[wordCount].count = 1;
            wordCount++;
        }
    }
    
    // Close the file
    fclose(file);
    
    // Display results
    printf("Word frequencies in %s:\n", argv[1]);
    for (int i = 0; i < wordCount; i++) {
        printf("%s: %d\n", wordList[i].word, wordList[i].count);
    }
    
    return 0;
}