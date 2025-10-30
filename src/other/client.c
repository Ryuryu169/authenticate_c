#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// Sends a message to the server
// Returns 0 on success, -1 on failure
// Print received message to stdout

int main(int argc, char *argv[])
{
    int sock_fd;
    struct sockaddr_in server_addr;
    char buf[1024];
    int bytes_read;

    // Create a socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("Error occur in socket()");
        exit(1);
    }

    // Connect to the server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(7500);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error occur in connect()");
        exit(1);
    }

    // Send a message to the server
    char *msg = "Hello from client!\n";
    write(sock_fd, msg, strlen(msg));

    // Read the server's response
    while ((bytes_read = read(sock_fd, buf, 1024)) > 0) {
        printf("%s", buf);
    }

    // Close the connection
    close(sock_fd);
    return 0;
}