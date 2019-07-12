#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main () {
    char server_address[] = "192.168.0.2";
    char message[] = "Zdravstvuyte from server";
    char buf_for_recv[100];
    char *ip;
    int server_socket_fd, port = 8081, bytes, struct_len;
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

    if (bind(server_socket_fd, (struct sockaddr *)&server_info, sizeof(server_info)) < 0) {
	perror("error in function bind()!\n");
	close(server_socket_fd);
	return 0;
    }
    ip = inet_ntoa(server_info.sin_addr);
    struct_len = sizeof(client_info);

    while (1) {
	bytes = recvfrom(server_socket_fd, buf_for_recv, 100, 0, (struct sockaddr *)&client_info, (socklen_t *)&struct_len);
	if (bytes <= 0) {
	    perror("");
	    printf("error in function recvfrom(), received %d bytes!\n", bytes);
	} else {
	    printf("SERVER(%s:%d) received %d bytes from client(%s:%d): %s\n", 
	    ip, ntohs(server_info.sin_port), bytes, inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port), buf_for_recv);
	}
	bytes = sendto(server_socket_fd, message, sizeof(message), 0, (struct sockaddr *)&client_info, struct_len);
	if (bytes <= 0) {
	    perror("");
	    printf("error in function sendto(), sended %d bytes!\n", bytes);
	} else {
	    printf("SERVER(%s:%d) sended %d bytes to client(%s:%d)\n", 
	    ip, ntohs(server_info.sin_port), bytes, inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));
	}
    }
    close(server_socket_fd);
    return 0;
}