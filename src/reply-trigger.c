#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>          // for close()
#include "networking.h"      // your BSD-sockets-based networking functions

int verbose = 0; // Global verbose level

int main(int argc, char** argv) {
    const int port = 8000;
    const char* ip = "127.0.0.1";

    // Receive verbosity level from cmdline
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-v") == 0) verbose = 1;
        }
    }

    // Init socket and connect to server
    int sock = connect_to_ip(ip, port);
    if (sock < 0) {
        return 1;
    }

    // Make buffers to store packet data
    char* send_buf = (char*)calloc(MAX_PKT_LEN, sizeof(char));
    char* recv_buf = (char*)calloc(MAX_PKT_LEN, sizeof(char));

    if (!send_buf || !recv_buf) {
        fprintf(stderr, "Memory allocation failed\n");
        close(sock);
        return 1;
    }

    // Send "HelloSecretString"
    const char* msg = "HelloSecretString\n";
    strcpy(send_buf, msg);
    send_packet(sock, send_buf, strlen(msg));

    // Receive response
    recv_packet(sock, recv_buf);

    const char* expected = "SuperSecretSecret\n";
    if (strcmp(recv_buf, expected) == 0) puts("Success");
    else puts("Failure");

    // Cleanup
    free(send_buf);
    free(recv_buf);
    close(sock);
    return 0;
}
