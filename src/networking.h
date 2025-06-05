int initialize_winsock();
SOCKET connect_to_ip(const char* ip, int port);
int send_packet(SOCKET sock, char* buf, int len);
int recv_packet(SOCKET sock, char* buf);