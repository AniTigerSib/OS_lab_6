#include "client.h"

#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main() {
	int status, client_fd;
	struct sockaddr_in serv_addr;
	char buffer[1024] = { 0 };

	pthread_t hThreadListner;

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

	pthread_create(&hThreadListner, NULL, Listner, &client_fd);

	while (1) {
		fgets(buffer, BUFFER_SIZE, stdin);
		buffer[strlen(buffer) - 1] = '\0';
		send(client_fd, buffer, strlen(buffer), 0);
		if (strcmp(buffer, "close") == 0) {
			break;	
		}
	}

	pthread_exit(0);

	// send(client_fd, hello, strlen(hello), 0);
	// printf("Hello message sent\n");
	// valread = read(client_fd, buffer, 1024 - 1);
	// printf("%s\n%d", buffer, valread);
	// send(client_fd, "close", strlen("close"), 0);

	close(client_fd);
	return 0;
}

void ClearBuffer(char* buffer) {
    memset(buffer, 0, BUFFER_SIZE);
}

void *Listner(void* arg) {
	int* p_client_fd = (int*)arg;
	char buffer[BUFFER_SIZE] = { 0 };
	int valread = 0;
	while (1) {
		valread = read(*p_client_fd, buffer, BUFFER_SIZE - 1);
		if (valread != 0) {
			if (strcmp(buffer, "close") == 0) {
				break;
			}
			printf("%s\n", buffer);
			ClearBuffer(buffer);
		}
	}
	return NULL;
}