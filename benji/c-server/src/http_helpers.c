#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "http_helpers.h"

char* get_http_method(char* str){

	static char temp_str[8];
        int i = 0;
        for(i = 0; i < 8; i++){
                if(str[i] == ' ' || str[i] == '\0'){
                        break;
                }else{
                        temp_str[i] = str[i];
                }
        }
        temp_str[i] = '\0';
        return temp_str;
}

char* get_http_path(char* str){

	int i = 0;
	int i2 = 0;
	bool method_extracted = false;
	bool is_path = false;
	static char temp_str[1024];

	for(i = 0; i < 1024; i++){
		if(str[i] == ' '){
			method_extracted = true;
		}
		if(str[i] == '/' && method_extracted){
			is_path = true;
			temp_str[i2++] = str[i];
		}
		else if((str[i] == ' ' && method_extracted && is_path) || str[i] == '\0'){
			break;
		}else if(is_path){
			temp_str[i2++] = str[i];
		}
	}
	temp_str[i2] = '\0';
	return temp_str;
}

char* get_http_version(char* str){

	size_t space_count = 0;
	static char temp_str[1024];
	int i = 0;
	int i2 = 0;
	while(space_count < 2){
		if(str[i++] == ' '){
			space_count++;
		}
	}
	while(str[i] != ' ' && str[i] != '\0' && str[i] != '\n'){
		if(i2 < sizeof(temp_str) - 1){
			temp_str[i2++] = str[i++];
		}else{
			break;
		}
	}
	temp_str[i2] = '\0';
	return temp_str;
}

char* create_http_response(char* str){
	char* method = get_http_method(str);
	char* path = get_http_path(str);
	char* version = get_http_version(str);

	if(strcmp(method, "GET") == 0){

		FILE* file = fopen(path + 1, "r");
		if(!file){
			char* not_found = malloc(256);
			snprintf(not_found, 256,
				"%s 404 Not Found\r\n"
				"Content-Type: text/playin\r\n"
				"Content-Length: 13\r\n"
				"Connection: close\r\n"
				"\r\n"
				"404 Not Found", version);
			return not_found;
		}

		fseek(file, 0, SEEK_END);
		long length = ftell(file);
		fseek(file, 0, SEEK_SET);

		char* body = malloc(length + 1);
		fread(body, 1, length, file);
		body[length] = '\0';
		fclose(file);

		char* response = malloc(length + 512);
		snprintf(response, length + 512,
			"%s 200 OK\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: %ld\r\n"
			"Connection: close\r\n"
			"\r\n"
			"%s", version, length, body);
		free(body);
		return response;
	}
}
