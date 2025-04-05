#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include "http_helpers.h"


int main(){

	// GLOBALS

	const int PORT = 8080;

	struct sockaddr_in address = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = INADDR_ANY,
		.sin_port = htons(PORT)
	};

	// SERVER SETUP

	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd == -1){
		perror("Server socket creation failed\n");
		return 1;
	}

	int server_did_bind = bind(server_fd,(struct sockaddr *)&address, sizeof(address));
	if(server_did_bind == -1){
		perror("Bind failed\n");
		return 1;
	}

	int server_listening = listen(server_fd, 10);
	if(server_listening == -1){
		printf("Listen failed\n");
		return 1;
	}else{
		printf("Server is listening on port %d\n", PORT);
	}

	// MAIN LOOP

	while(1){
		char buffer[1024] = {0};
		int client_fd = accept(server_fd, (struct sockaddr *)&address, &(socklen_t){sizeof(address)});
		ssize_t recieved_bytes = read(client_fd, buffer, sizeof(buffer) - 1);
		if(recieved_bytes < 0){
			perror("Read msg from client failed\n");
		}else{
			printf("%s\n", get_http_method(buffer));
			printf("%s\n", get_http_path(buffer));
		}
		close(client_fd);
	}
	close(server_fd);
	return 0;
}
