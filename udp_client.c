#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main () {
    int bytes, port = 8080, server_socket_fd, struct_len;
    char server_address[] = "192.168.1.2";
    char message[] = "Kon'nichiwa from Client";
    char buf_for_recv[100];
    struct sockaddr_in server_info, client_info;

    memset(&server_info, 0, sizeof(server_info));

    if (inet_aton(server_address, (struct in_addr *)&server_info.sin_addr) == 0) {
	printf("Address of server is invalid!\n");
	return 0;
    }

    server_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket_fd < 0) {
	printf("error in function socket()!\n");
	return 0;
    }
    
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(port);
    //server_info.sin_addr.s_addr = htonl(server_info.sin_addr.s_addr); //Does it have significanсe?
    struct_len = sizeof(server_info);

    bytes = sendto(server_socket_fd, message, sizeof(message), 0, (struct sockaddr *)&server_info, sizeof(server_info));
    if (bytes <= 0) {
	printf("error in function sendto(), sended %d bytes!\n", bytes);
    } else {
	printf("CLIENT: sended %d bytes to server ()\n", bytes);
    }

    bytes = recvfrom(server_socket_fd, buf_for_recv, 100, 0, (struct sockaddr *)&server_info, (socklen_t *)&struct_len);
    if (bytes <= 0) {
	printf("error in function recvfrom(), received %d bytes!\n", bytes);
    } else {
	printf("CLIENT: receiveded %d bytes from server (): %s\n", bytes, buf_for_recv);
    }

    close(server_socket_fd);
    return 0;
}