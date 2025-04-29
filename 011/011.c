#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

// Signal handler for SIGALRM (alarm trigger)
void alarm_handler(int sig) {
    printf("\nALARM! Time's up!\n");
    exit(0);
}

// Signal handler for SIGINT (Ctrl+C)
void interrupt_handler(int sig) {
    printf("\nAlarm canceled!\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    // Check for valid arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <seconds>\n", argv[0]);
        exit(1);
    }

    // Convert input to integer
    int seconds = atoi(argv[1]);
    if (seconds <= 0) {
        fprintf(stderr, "Error: Please enter a positive integer\n");
        exit(1);
    }

    // Register signal handlers
    if (signal(SIGALRM, alarm_handler) == SIG_ERR) {
        perror("Error registering alarm handler");
        exit(1);
    }

    if (signal(SIGINT, interrupt_handler) == SIG_ERR) {
        perror("Error registering interrupt handler");
        exit(1);
    }

    // Set the alarm
    alarm(seconds);
    printf("Alarm set for %d seconds\n", seconds);
    printf("Press Ctrl+C to cancel\n");

    // Wait for alarm to trigger
    while(1) {
        pause();  // Wait for any signal
    }

    return 0;
}