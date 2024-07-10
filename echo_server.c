#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define PORT 8888
#define BUFFER_SIZE 1024
#define MAX_ACTIVE_CLIENTS 2
#define MAX_TOTAL_CLIENTS 3

pthread_t client_threads[MAX_TOTAL_CLIENTS];
int active_client_count = 0;
int total_client_count = 0;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg) {
    int new_socket = *((int *)arg);
    free(arg);
    char buffer[BUFFER_SIZE] = {0};
    int valread;

    time_t start_time = time(NULL);

    printf("Client connected,the socket: %d\n", new_socket);

    while (difftime(time(NULL), start_time) < 10) {
        valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            printf("Client disconnected,the socket: %d\n", new_socket);
            break;
        }
        printf("Received from client %d: %s\n", new_socket, buffer);
        send(new_socket, buffer, valread, 0);
        printf("Echoed to client %d: %s\n", new_socket, buffer);
    }

    close(new_socket);
    printf("Connection closed, socket: %d\n", new_socket);

    pthread_mutex_lock(&client_mutex);
    active_client_count--;
    pthread_mutex_unlock(&client_mutex);

    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Waiting\n");

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr )&address, (socklen_t)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        pthread_mutex_lock(&client_mutex);

        if (total_client_count >= MAX_TOTAL_CLIENTS) {
            printf("Max client reached that is why connection rejected: %d\n", new_socket);
            close(new_socket);
        } else if (active_client_count < MAX_ACTIVE_CLIENTS) {
            int *new_sock = malloc(sizeof(int));
            *new_sock = new_socket;
            pthread_create(&client_threads[total_client_count], NULL, handle_client, (void *)new_sock);
            active_client_count++;
            total_client_count++;
        } else {
            printf("Limit is max, client that is waiting: %d\n", new_socket);
            int *new_sock = malloc(sizeof(int));
            *new_sock = new_socket;
            pthread_create(&client_threads[total_client_count], NULL, handle_client, (void *)new_sock);
            total_client_count++;
        }

        pthread_mutex_unlock(&client_mutex);
    }

    return 0;
}