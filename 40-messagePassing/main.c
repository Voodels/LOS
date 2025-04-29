#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg_buffer {
    long msg_type;
    char msg_text[100];
} message;

int main() {
    key_t key = ftok("chat", 65);
    int msgid = msgget(key, 0666|IPC_CREAT);
    
    if(fork() == 0) { // User 1
        while(1) {
            printf("User1 Enter: ");
            fgets(message.msg_text, 100, stdin);
            message.msg_type = 1;
            msgsnd(msgid, &message, sizeof(message), 0);
        }
    } else if(fork() == 0) { // User 2
        while(1) {
            msgrcv(msgid, &message, sizeof(message), 1, 0);
            printf("User2 Received: %s", message.msg_text);
        }
    } else { // Parent
        wait(NULL);
        msgctl(msgid, IPC_RMID, NULL);
    }
    return 0;
}