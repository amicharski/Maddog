#include <stdio.h>
#include <stdlib.h>
#include "../include/shell.h"

void print_to_shell(char* text){
    printf("MADDOGSHELL>>> %s\n", text);
}

void print_error(char* msg){
    printf("MADDOGERROR>>> %s", msg);
    exit(1);
}