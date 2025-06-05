#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>              // for close()
#include <arpa/inet.h>           // for inet_addr(), htons()
#include <sys/socket.h>          // for socket functions
#include <netinet/in.h>          // for sockaddr_in
#include <errno.h>               // for errno
#include "networking.h"

extern int verbose;

// Attempt connection to the given IP and port
int connect_to_ip(const char* ip, int port) {
    int sock;
    struct sockaddr_in server;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        fprintf(stderr, "Socket creation failed with error: %s\n", strerror(errno));
        return -1;
    }

    // Prepare the sockaddr_in structure
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &server.sin_addr) <= 0) {
        fprintf(stderr, "Invalid IP address: %s\n", ip);
        close(sock);
        return -1;
    }

    // Attempt to connect
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        fprintf(stderr, "Connection failed with error: %s\n", strerror(errno));
        close(sock);
        return -1;
    }

    if (verbose) printf("Successfully connected to %s on port %d\n", ip, port);
    return sock;
}

int send_packet(int sock, char* buf, int len) {
    int sent = send(sock, buf, len, 0);
    if (sent < 0) {
        fprintf(stderr, "Send failed with error: %s\n", strerror(errno));
        return 1;
    }

    if (verbose) printf("Sent %d bytes: %.*s\n", sent, sent, buf);
    return 0;
}

int recv_packet(int sock, char* buf) {
    int received = recv(sock, buf, MAX_PKT_LEN, 0);
    if (received < 0) {
        fprintf(stderr, "Receive failed with error: %s\n", strerror(errno));
        return 1;
    } else if (received == 0) {
        fprintf(stderr, "Connection closed by server\n");
        return 1;
    }

    if (verbose) printf("Received %d bytes: %.*s\n", received, received, buf);
    return received;
}
