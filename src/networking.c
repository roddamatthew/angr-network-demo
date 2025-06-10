#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>              // for close()
#include <arpa/inet.h>           // for inet_addr(), htons()
#include <sys/socket.h>          // for socket functions
#include <netinet/in.h>          // for sockaddr_in
#include <errno.h>               // for errno4
#include <ctype.h>
#include "networking.h"

extern int verbose;

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int is_valid_ip(const char *ip) {
    if (!ip) return 0;
    
    int len = strlen(ip);
    if (len == 0) return 0;
    
    // Check for leading or trailing dots
    if (ip[0] == '.' || ip[len-1] == '.') {
        return 0;
    }
    
    // Check for consecutive dots
    for (int i = 0; i < len - 1; i++) {
        if (ip[i] == '.' && ip[i+1] == '.') {
            return 0;
        }
    }
    
    char *ip_copy = malloc(len + 1);
    if (!ip_copy) return 0;
    strcpy(ip_copy, ip);
    
    char *token;
    int count = 0;
    
    // Split by dots
    token = strtok(ip_copy, ".");
    while (token != NULL) {
        count++;
        
        // Check if we have more than 4 octets
        if (count > 4) {
            free(ip_copy);
            return 0;
        }
        
        // Check if token is empty (this shouldn't happen now, but keep for safety)
        if (strlen(token) == 0) {
            free(ip_copy);
            return 0;
        }
        
        // Check if token has more than 3 digits
        if (strlen(token) > 3) {
            free(ip_copy);
            return 0;
        }
        
        // Check if all characters are digits
        for (int i = 0; i < strlen(token); i++) {
            if (!isdigit(token[i])) {
                free(ip_copy);
                return 0;
            }
        }
        
        // Check for leading zeros (except for "0" itself)
        if (strlen(token) > 1 && token[0] == '0') {
            free(ip_copy);
            return 0;
        }
        
        // Convert to integer and check range
        int num = atoi(token);
        if (num < 0 || num > 255) {
            free(ip_copy);
            return 0;
        }
        
        token = strtok(NULL, ".");
    }
    
    free(ip_copy);
    
    // Must have exactly 4 octets
    return (count == 4);
}

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
