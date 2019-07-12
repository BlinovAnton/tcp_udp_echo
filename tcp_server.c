#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main () {
    int sockfd_of_server, new_sock_fd, port = 8080, bytes;
    struct sockaddr_in struct_with_server_address, client_address;
    char buf_for_recv[100];
    char *client_address_str;
    char *ip;
    char message[] = "Alloha from server!";
    char server_address[] = "192.168.0.2"; //but in reality server ip using as 127.0.0.1

    memset(&struct_with_server_address, 0, sizeof(struct_with_server_address));

    if (inet_aton(server_address, (struct in_addr *)&struct_with_server_address.sin_addr) == 0) {
	printf("Address of server is invalid!\n");
	return 0;
    }

    sockfd_of_server = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_of_server < 0) {
	printf("error in function socket()!\n");
	return 0;
    }
    struct_with_server_address.sin_family = AF_INET;
    struct_with_server_address.sin_port = htons(port);
    ip = inet_ntoa(struct_with_server_address.sin_addr);

    if (bind(sockfd_of_server, (struct sockaddr *)&struct_with_server_address, sizeof(struct_with_server_address)) < 0) {
	printf("error in function bind!\n");
	perror(":::");
	close(sockfd_of_server);
	return 0;
    }

    listen(sockfd_of_server, 1);

    int struct_len = sizeof(client_address);
while (1) {
    new_sock_fd = accept(sockfd_of_server, (struct sockaddr *)&client_address, (socklen_t *)&struct_len);
    if (new_sock_fd < 0) {
	printf("error in function accept()!\n");
	close(sockfd_of_server);
	return 0;
    }

    client_address_str = inet_ntoa(client_address.sin_addr);
    if ((bytes = recv(new_sock_fd, buf_for_recv, 100, 0)) <= 0) { // 100 - magic constant
	printf("error in receiving data from client (%s), recv-ed %d bytes!\n", client_address_str, bytes);
    } else {
	printf("server (%s:%d) received %d bytes from client (%s:%d): %s\n", 
	ip, ntohs(struct_with_server_address.sin_port), bytes, client_address_str, ntohs(client_address.sin_port), buf_for_recv);
    }
    if ((bytes = send(new_sock_fd, message, sizeof(message), 0)) <= 0) {
	printf("error in sending data to clint, sended %d bytes!\n", bytes); 
    } else {
	printf("server(%s:%d) send %d bytes to client (%s:%d)\n", 
	ip, ntohs(struct_with_server_address.sin_port), bytes, client_address_str, ntohs(client_address.sin_port));
    }
    sleep(10);
    close(new_sock_fd);
}
    close(new_sock_fd);
    close(sockfd_of_server);
    return 0;
}