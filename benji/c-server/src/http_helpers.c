#include "http_helpers.h"


char temp_str[8];

char* get_http_method(char* str){
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

