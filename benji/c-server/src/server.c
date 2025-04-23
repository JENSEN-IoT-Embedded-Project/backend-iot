#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#include "http_helpers.h"

void* thread_function(void *arg);
void handle_sigint(int sig);

int server_fd;

int main(){

	signal(SIGINT, handle_sigint);

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

		int client_socket = accept(server_fd, (struct sockaddr *)&address, &(socklen_t){sizeof(address)});
		if(client_socket == -1){
			perror("Accept failed");
		}else{
			pthread_t new_thread;
			int* client_fd = malloc(sizeof(int));
			*client_fd = client_socket;
			int thread_result = pthread_create(&new_thread, NULL, thread_function, (void*)client_fd);
			if(thread_result != 0){
				perror("Thread creation failed\n");
				free(client_fd);
			}else{
				pthread_detach(new_thread);
			}
		}
	}
	close(server_fd);
	return 0;
}

void* thread_function(void *arg){
	char buffer[1024] = {0};

	int client_socket = *((int*)arg); // convert to exact type
	free(arg);

	int read_success = read(client_socket, buffer, sizeof(buffer) - 1);

	if(read_success == -1){
		perror("Read data failed\n");
	}else if(read_success == 0){
		printf("Client disconnected\n");
	}else{
		if(read_success < sizeof(buffer)){
			buffer[read_success] = '\0';
		}
		char* http_response = create_http_response(buffer);

		int msg_sent = send(client_socket, http_response, strlen(http_response), MSG_NOSIGNAL);
		if(msg_sent == -1){
			perror("Message failed to send to client\n");
		}
	}
	close(client_socket);
}

void handle_sigint(int sig){
	printf("\nCaught SIGINT, shutting down server...\n");
	if(server_fd != -1){
		close(server_fd);
	}
	exit(0);
}
