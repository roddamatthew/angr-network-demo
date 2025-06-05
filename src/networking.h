#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_PKT_LEN 32

#endif //CONSTANTS_H

int initialize_winsock();
SOCKET connect_to_ip(const char* ip, int port);
int send_packet(SOCKET sock, char* buf, int len);
int recv_packet(SOCKET sock, char* buf);