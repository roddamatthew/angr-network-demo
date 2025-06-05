#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int verbose;

int initialize_winsock() {
    WSADATA wsaData;
    int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (ret != 0) {
        fprintf(stderr, "WSAStartup failed with error: %d\n", ret);
        return 1;
    }
    return 0;
}

// Attempt connection to the given IP and port
SOCKET connect_to_ip(const char* ip, int port) {
    SOCKET sock;
    struct sockaddr_in server;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "Socket creation failed with error: %d\n", WSAGetLastError());
        return INVALID_SOCKET;
    }

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);

    // Attempt to connect
    int ret = connect(sock, (struct sockaddr*)&server, sizeof(server));
    if (ret == SOCKET_ERROR) {
        fprintf(stderr, "Connection failed with error: %d\n", WSAGetLastError());
        closesocket(sock);
        return INVALID_SOCKET;
    }

    if (verbose) printf("Successfully connected to %s on port %d\n", ip, port);
    return sock;
}

int send_packet(SOCKET sock, char* buf, int len) {
    int sent = send(sock, (const char*) buf, (int)len, 0);
    if (sent == SOCKET_ERROR) {
        fprintf(stderr, "Send failed with error: %d\n", WSAGetLastError());
        return 1;
    }

    if (verbose) {
        printf("Sent %d bytes: ", sent);
        for (int j = 0; j < len; j++) {
            printf("%02X ", (uint8_t)data[j]);
        }
        printf("\n");
    }

    return 0;
}

int recv_packet(SOCKET sock, char* buf) {
    int received = recv(sock, (char*)response, MAX_PACKET_SIZE, 0);
    if (received == SOCKET_ERROR) {
        fprintf(stderr, "Receive failed with error: %d\n", WSAGetLastError());
        return 1;
    } else if (received == 0) {
        fprintf(stderr, "Connection closed by server\n");
        return 1;
    }

    if (verbose) {
        printf("Received %d bytes: ", received);
        for (int i = 0; i < received; ++i) {
            printf("%02X ", response[i]);
        }
        printf("\n");
    }

    return received;
}