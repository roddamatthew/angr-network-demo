#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PKT_LEN 32
int verbose = 0; // Global verbose level

int main(int argc, char** argv) {
    const int port = 8000;
    const char* ip = "localhost";
    
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
    char send_buf[MAX_PKT_LEN];
    char recv_buf[MAX_PKT_LEN];

    // Send "HelloSecretString"
    strcpy()

    // Recieve response

    // If response == "SuperSecretSecret"
        // print "Success"
    // else
        // print "Failure"
    
    return 0;
}