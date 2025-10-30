#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    int listen_fd, conn_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("Error occured in socket()");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(7500);
    if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error occured in bind()");
        exit(1);
    }

    if(listen(listen_fd, 10) < 0) {
        perror("Error occured in listen()");
        exit(1);
    }

    printf("Listening on port %d...\n", 7500);

    while(1){
        client_len = sizeof(client_addr);
        conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len);
        if (conn_fd < 0) {
            perror("Error occured in accept()");
            continue;
        }
        printf("Connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Reads the data from the client
        char buf[1024];
        int bytes_read;
        while ((bytes_read = read(conn_fd, buf, 1024)) > 0) {
            write(conn_fd, buf, bytes_read);
            printf("%s", buf);
        }

        // Sends the data to the client
        char *msg = "Hello from server!\n";
        write(conn_fd, msg, strlen(msg));

        // Close the connection
        close(conn_fd);
        printf("Connection closed\n");
    }
    return 0;
}