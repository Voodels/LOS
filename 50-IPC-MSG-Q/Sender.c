#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

// Define the message structure
struct msg_buffer {
    long msg_type;
    char msg_text[100];
} message;

int main() {
    key_t key = ftok("msgfile", 65); // Generate a unique key

    // Create or access the message queue
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        return 1;
    }

    // Send a message to Process 2
    message.msg_type = 1; // Message type (must be > 0)
    strcpy(message.msg_text, "Are you hearing me?");
    if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
        perror("msgsnd failed");
        return 1;
    }
    printf("Process 1: Sent message: '%s'\n", message.msg_text);

    // Wait for a reply from Process 2
    if (msgrcv(msgid, &message, sizeof(message.msg_text), 2, 0) == -1) {
        perror("msgrcv failed");
        return 1;
    }
    printf("Process 1: Received reply: '%s'\n", message.msg_text);

    // Send another message
    strcpy(message.msg_text, "I can hear you too.");
    message.msg_type = 3;
    if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
        perror("msgsnd failed");
        return 1;
    }
    printf("Process 1: Sent final message: '%s'\n", message.msg_text);

    return 0;
}