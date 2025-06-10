#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_PKT_LEN 32

#endif //CONSTANTS_H

int connect_to_ip(const char* ip, int port);
int send_packet(int sock, char* buf, int len);
int recv_packet(int sock, char* buf);
int is_valid_ip(const char *ip);