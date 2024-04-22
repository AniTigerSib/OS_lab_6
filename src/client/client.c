#include "client.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	char hello[] = "Hello from client";
	char buffer[1024] = { 0 };
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, IP_ADDR, &serv_addr.sin_addr)
		<= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	send(client_fd, hello, strlen(hello), 0);
	printf("Hello message sent\n");
	valread = read(client_fd, buffer, 1024 - 1);
	printf("%s\n%d", buffer, valread);
	send(client_fd, "close", strlen("close"), 0);

	close(client_fd);
	return 0;
}
