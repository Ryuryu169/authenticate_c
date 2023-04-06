#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define PORT 80

int main(int argc, char *argv[]) {
    int listen_fd, conn_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buf[BUF_SIZE];
    int bytes_read;

    // Create a socket for listening
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket");
        exit(1);
    }

    // Bind the socket to a port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);
    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(listen_fd, 10) < 0) {
        perror("listen");
        exit(1);
    }

    printf("Listening on port %d...\n", PORT);

    while (1) {
        // Accept a connection from a client
        client_len = sizeof(client_addr);
        conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len);
        if (conn_fd < 0) {
            perror("accept");
            continue;
        }

        printf("Connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Echo back any data received from the client
        while ((bytes_read = read(conn_fd, buf, BUF_SIZE)) > 0) {
            write(conn_fd, buf, bytes_read);
        }

        // Close the connection
        close(conn_fd);
    }

    return 0;
}