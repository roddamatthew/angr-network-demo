#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "networking.h"

int verbose = 0; // Global verbose level

int main(int argc, char** argv) {
    const int port = 8000;
    const char* ip = "127.0.0.1";
    
    // Recieve verbosity level from cmdline
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-v") == 0) verbose = 1;
        }
    }

    if (initialize_winsock() != 0) {
        return 1;
    }

    // Init socket and connect to server
    SOCKET sock = connect_to_ip(ip, port);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    // Make buffers to store packet data
    char* send_buf = (char*)calloc(MAX_PKT_LEN, sizeof(char));
    char* recv_buf = (char*)calloc(MAX_PKT_LEN, sizeof(char));

    // Send "HelloSecretString"
    const char* msg = "HelloSecretString\n";
    strcpy(send_buf, msg);
    send_packet(sock, send_buf, strlen(msg));

    // Recieve response
    recv_packet(sock, recv_buf);

    const char* expected = "SuperSecretSecret\n";
    if (strcmp(recv_buf, expected) == 0) puts("Success");
    else puts("Failure");
    
    return 0;
}