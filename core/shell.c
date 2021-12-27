#include <stdio.h>
#include <stdlib.h>
#include "../include/shell.h"

static int line_number = 1;

void print_to_shell(char* text){
    printf("MADDOGSHELL>>> %s\n", line_number, text);
}

void print_error(char* msg){
    printf("MADDOGERROR (ERROR AT LINE %d)>>> %s", msg);
    exit(1);
}