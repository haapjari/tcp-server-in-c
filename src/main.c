#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

void main() {
    // Create a Socket or IPV4, with Stream type and Default Protocol. (TCP)
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        perror("could not create socket");
        exit(EXIT_FAILURE);
    }

    const int port_number = 8080;

    // Define a "sockaddr_in" structure, which holds socket address information.
    struct sockaddr_in addr;

    addr.sin_family = AF_INET ;          // Address family for IPv4.
    addr.sin_port = htons(port_number);  // Port Number in Network Byte OrderPort number (in network byte order). 
    addr.sin_addr.s_addr = INADDR_ANY;   // Address,  binds to all local network interfaces.
    memset(&(addr.sin_zero), '\0', 8);   // Zero Out, the rest of the structure.

    // Bind the Socket to the Address and Port Number, specified in "addr".
    bind(s, (struct sockaddr *)&addr, sizeof(addr));

    // Start Listening for Incoming Connections (with a backlog of 10 connections)
    if (listen(s, 10) == -1) {
        perror("listen has failed");
        close(s);
        exit(EXIT_FAILURE);
    }

    // Accept an incoming connection, creating a new socket for the connection.
    int client_fd = accept(s, NULL, NULL);
    if (client_fd == -1) {
        perror("accept has failed");
        close(s);
        exit(EXIT_FAILURE);
    }

    // Loop to keep reading data fomr the client and echoing it back.
    while (1) {
        char buffer[256];
        ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer), 0);

        // Check, if the client closed the connection, or if there was an error.
        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                printf("Client disconnected.\n");
            } else {
                perror("recv failed");
            }
            break;
        }

        // Null-Terminate the received message, to safely use string functions.
        buffer[bytes_read] = '\0';

        // Echo the data back to the client.
        ssize_t bytes_sent = send(client_fd, buffer, bytes_read, 0);
        if (bytes_sent == -1) {
            perror("send failed");
            break;
        }
    }

    // Close the client socket, after breaking out of the loop.
    close(client_fd);
    
    // Close the listening socket.
    close(s);
}