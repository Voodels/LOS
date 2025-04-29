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
    key_t key = ftok("msgfile", 65); // Generate the same key as Program 1

    // Access the message queue
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        return 1;
    }

    // Receive the initial message from Process 1
    if (msgrcv(msgid, &message, sizeof(message.msg_text), 1, 0) == -1) {
        perror("msgrcv failed");
        return 1;
    }
    printf("Process 2: Received message: '%s'\n", message.msg_text);

    // Send a reply to Process 1
    strcpy(message.msg_text, "Loud and Clear");
    message.msg_type = 2;
    if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
        perror("msgsnd failed");
        return 1;
    }
    printf("Process 2: Sent reply: '%s'\n", message.msg_text);

    // Receive the final message from Process 1
    if (msgrcv(msgid, &message, sizeof(message.msg_text), 3, 0) == -1) {
        perror("msgrcv failed");
        return 1;
    }
    printf("Process 2: Received final message: '%s'\n", message.msg_text);

    return 0;
}