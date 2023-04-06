#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/md5.h>
#include <pthread.h>

#define CHALLENGE_LEN 32
#define PORT 80

// Function to generate a random challenge string
void generate_challenge(char *challenge)
{
    // Implement your own logic to generate a random challenge
    // Here we use a fixed challenge for demonstration purposes
    strncpy(challenge, "12345678901234567890123456789012", CHALLENGE_LEN);
}

// Function to calculate the expected response string for a given challenge and password
void calculate_expected_response(char *challenge, char *password, char *expected_response)
{
    // Calculate the MD5 hash of the concatenated challenge and password
    char concat[CHALLENGE_LEN + strlen(password) + 1];
    strcpy(concat, challenge);
    strcat(concat, password);

    unsigned char md5_digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char *)concat, strlen(concat), md5_digest);

    // Convert the binary digest to a hexadecimal string
    char hex_digest[2 * MD5_DIGEST_LENGTH + 1];
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(&hex_digest[i * 2], "%02x", md5_digest[i]);
    }

    strcpy(expected_response, hex_digest);
}

/*
// Thread function to handle client connections
void *handle_client(void *arg)
{
    int client_socket = *(int *)arg;

    // Generate a random challenge string
    char challenge[CHALLENGE_LEN + 1];
    generate_challenge(challenge);

    // Print the challenge string for demonstration purposes
    printf("Challenge: %s\n", challenge);

    char password[] = "mysecret"; // Change this to the known password for the client

    char expected_response[2 * MD5_DIGEST_LENGTH + 1];
    calculate_expected_response(challenge, password, expected_response);

    // Receive the response string from the client
    char response[2 * MD5_DIGEST_LENGTH + 1];
    ssize_t response_size = recv(client_socket, response, sizeof(response) - 1, 0);
    if (response_size < 0) {
        perror("Failed to receive response from client");
        exit(EXIT_FAILURE);
    }
    response[response_size] = '\0';

    // Verify the response string
    if (strcmp(expected_response, response) == 0) {
        printf("Authentication succeeded\n");
        send(client_socket, "OK", 2, 0);
    } else {
        printf("Authentication failed\n");
        send(client_socket, "FAIL", 4, 0);
    }

    // Close the client socket
    close(client_socket);

    return NULL;
}*/

int main()
{
    char password[] = "mysecret"; // Change this to the known password for the client

    // Create a socket for the server
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Failed to create server socket");
        exit(EXIT_FAILURE);
    }

    // Bind the server socket to a specific port
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Failed to bind server socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 1) < 0) {
        perror("Failed to listen for incoming connections");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Generate a random challenge string
        char challenge[CHALLENGE_LEN + 1];
        generate_challenge(challenge);

        // Print the challenge string for demonstration purposes
        printf("Challenge: %s\n", challenge);

        // Accept a client connection
        struct sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
        if (client_socket < 0) {
            perror("Failed to accept client connection");
            exit(EXIT_FAILURE);
        }

        // Receive the response string from the client
        char response[2 * MD5_DIGEST_LENGTH + 1];
        ssize_t response_size = recv(client_socket, response, sizeof(response) - 1, 0);
        if (response_size < 0) {
            perror("Failed to receive response from client");
            exit(EXIT_FAILURE);
        }
        response[response_size] = '\0';

        // Calculate the expected response string
        char expected_response[2 * MD5_DIGEST_LENGTH + 1];
        calculate_expected_response(challenge, password, expected_response);

        // Verify the response string
        if (strcmp(expected_response, response) == 0) {
            printf("Authentication succeeded\n");
            send(client_socket, "OK", 2, 0);
        } else {
            printf("Authentication failed\n");
            send(client_socket, "FAIL", 4, 0);
        }

        // Close the client socket
        //close(client_socket);
    }

    // Close the server socket
    //close(server_socket);

    return 0;
}