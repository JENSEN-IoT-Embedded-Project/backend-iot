#include <stdbool.h>

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
	static char temp_str[20];

	for(i = 0; i < 30; i++){
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
