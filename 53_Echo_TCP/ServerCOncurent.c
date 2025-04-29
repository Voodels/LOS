#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

void *handle_client(void *arg) {
    int sock = *(int *)arg;
    char buffer[1024] = {0};

    // Read data from the client
    int valread = read(sock, buffer, 1024);
    printf("Message from client: %s\n", buffer);

    // Send the same data back to the client
    send(sock, buffer, valread, 0);
    printf("Echoed message to client\n");

    close(sock);
    free(arg);
    pthread_exit(NULL);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Echo server listening on port %d\n", PORT);

    while (1) {
        // Accept a connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        // Create a new thread for the client
        int *sock_ptr = malloc(sizeof(int));
        *sock_ptr = new_socket;
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, sock_ptr);
        pthread_detach(thread_id);
    }

    close(server_fd);
    return 0;
}