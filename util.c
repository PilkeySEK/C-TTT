#include <string.h>

/*
    Converts a string containing decimal letters (0-9) into a int. This will ignore other letters in the string. Does not work with negative numbers
    @param str The string
    @returns The number
*/
long strtolong(char* str) {
    size_t len = strlen(str);
    long ret = 0;
    for(size_t i = 0; i < len; i++) {
        if(str[i] <= '9' && str[i] >= '0') {
            ret *= 10;
            ret += str[i] - '0';
        }
    }
    return ret;
}