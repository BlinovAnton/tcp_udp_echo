#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_IP_LENGTH 15

int main () {
    int i, bytes, port = 8080, socket_fd, connect_res;
    char server_address[][MAX_IP_LENGTH] = {"10.0.2.15", "192.168.1.2", "192.168.43.185", "127.0.0.1"};
    char message[] = "Ciao from Client!";
    char buf_for_recv[100];
    char *ip;
    struct sockaddr_in struct_of_server_address;

    memset(&struct_of_server_address, 0, sizeof(struct_of_server_address));

    ip = inet_ntoa(struct_of_server_address.sin_addr);

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	printf("CLIENT: error in function socket()!\n");
	return 0;
    }

    struct_of_server_address.sin_family = AF_INET;
    struct_of_server_address.sin_port = htons(port);
    //struct_of_server_address.sin_addr.s_addr = htonl(struct_of_server_address.sin_addr.s_addr); //Does it have significance?

    for (i = 0; i < sizeof(server_address) / MAX_IP_LENGTH; i++) {
	if (inet_aton(server_address[i], (struct in_addr *)&struct_of_server_address.sin_addr) == 0) {
	    printf("CLIENT: Address of server is invalid!\n");
	    return 0;
	}
	printf("CLIENT: try connect to %s - ", server_address[i]);
	connect_res = connect(socket_fd, (struct sockaddr *)&struct_of_server_address, sizeof(struct_of_server_address));
	if (connect_res < 0) {
	    printf("shit\n");
	} else {
	    printf("connected\n");
	    i = sizeof(server_address) / MAX_IP_LENGTH;
	}
    }
    if (connect_res < 0) {
	printf("CLIENT: error in connect()!\n");
	close(socket_fd);
	return 0;
    }

    bytes = send(socket_fd, message, sizeof(message), 0);
    if (bytes <= 0) {
	printf("CLIENT: error in function send, sended %d bytes!\n", bytes);
    } else {
	printf("CLIENT: sended %d bytes to server(%s:%d)\n", bytes, ip, ntohs(struct_of_server_address.sin_port));
    }
    bytes = recv(socket_fd, buf_for_recv, 100, 0); // 100 - magic constant
    if (bytes <= 0) {
	printf("CLIENT: error in function recv, recveived %d bytes!\n", bytes);
    } else {
	printf("CLIENT: recveived %d bytes from server(%s:%d): %s", bytes, ip, ntohs(struct_of_server_address.sin_port), buf_for_recv); //can't get ip of server to print it
    }
    close(socket_fd);
    return 0;
}