#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/tokenizer.h"
#include "../include/shell.h"

int i = 0;
char range_arr[40];
enum TokenType* tokens;

char peek_next(){
    return source[i+1];
}

char* peek_range(int range){
    if(range > 40){
        printf("INTERNAL ERROR>>> RANGE CANNOT BE GREATER THAN 40");
        exit(2);
    }
    for(int j = 0; j < range; j++){
        range_arr[j] = source[i+j+1];
    }

    return (char *) range_arr;
}

bool is_current_word(const char* word){
    for(int j = 0; j < strlen(word); j++){
        if(word[j] != source[i+j]){
            return false;
        }
    }
    i += strlen(word); //-1;
    return true;
}

bool is_digit(char ch){
    return ch >= '0' && ch <= '9';
}

bool is_hex(char ch){
    return ch >= '0' && ch <= '9' || ch >= 'a' && ch <= 'f' || ch >= 'A' && ch <= 'F';
}

bool is_bin(char ch){
    return ch == '0' || ch == '1';
}

bool is_oct(char ch){
    return ch >= '0' && ch <= '7';
}

bool is_alpha(char ch){
    return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_';
}

bool is_alphanumeric(char ch){
    return is_alpha(ch) || is_digit(ch);
}

bool consume_binary(){
    if(!is_current_word("0b")) return false;
    do {} while(is_bin(source[i++]));
    if(is_alphanumeric(source[i++])) return false;
    return true;
}

bool consume_octal(){
    if(!is_current_word("0c")) return false;
    do {} while(is_oct(source[i++]));
    if(is_alphanumeric(source[i++])) return false;
    return true;
}

bool consume_hex(){
    if(!is_current_word("0x")) return false;
    do {} while(is_hex(source[i++]));
    if(is_alphanumeric(source[i++])) return false;
    return true;
}

bool consume_int(){
    do {} while(is_digit(source[i++]));
    if(is_alpha(source[i++])) return false;
    return true;
}

char* tokenize(char* sourceCode){
    source = sourceCode;
    tokens = malloc(1024);
    do {
        char ch = sourceCode[i];
        printf("\nch: %c\n", ch);
        switch(ch){
            case '(':
                tokens[token_counter++] = LEFT_PAREN;
                print_to_shell("LEFT PARENTHESIS");
                break;
            case ')':
                tokens[token_counter++] = RIGHT_PAREN;
                print_to_shell("RIGHT PARENTHESIS");
                break;
            case '[':
                tokens[token_counter++] = LEFT_BRACKET;
                print_to_shell("LEFT BRACKET");
                break;
            case ']':
                tokens[token_counter++] = RIGHT_BRACKET;
                print_to_shell("RIGHT BRACKET");
                break;
            case '{':
                tokens[token_counter++] = LEFT_BRACE;
                print_to_shell("LEFT BRACE");
                break;
            case '}':
                tokens[token_counter++] = RIGHT_BRACE;
                print_to_shell("RIGHT BRACE");
                break;
            case '<':
                tokens[token_counter++] = LEFT_ANGLE;
                print_to_shell("LEFT ANGLE");
                break;
            case '>':
                tokens[token_counter++] = RIGHT_ANGLE;
                print_to_shell("RIGHT ANGLE");
                break;
            default:
                if(is_current_word("import")){
                    tokens[token_counter++] = IMPORT;
                    print_to_shell("IMPORT STATEMENT");
                } else if(is_current_word("signed")){
                    tokens[token_counter++] = SIGNED;
                    print_to_shell("SIGNED");
                } else if(is_current_word("unsigned")){
                    tokens[token_counter++] = UNSIGNED;
                    print_to_shell("UNSIGNED");
                } else if(is_current_word("public")){
                    tokens[token_counter++] = PUBLIC;
                    print_to_shell("PUBLIC");
                } else if(is_current_word("private")){
                    tokens[token_counter++] = PRIVATE;
                    print_to_shell("PRIVATE");
                } else if(is_current_word("protected")){
                    tokens[token_counter++] = PROTECTED;
                    print_to_shell("PROTECTED");
                } else if(is_current_word("namespace")){
                    tokens[token_counter++] = NAMESPACE;
                    print_to_shell("NAMESPACE DECLARATION");
                } else if(is_current_word("return")){
                    tokens[token_counter++] = RETURN;
                    print_to_shell("RETURN STATEMENT");
                } else if(is_current_word("esc")){
                    tokens[token_counter++] = ESCAPE;
                    print_to_shell("ESCAPE STATEMENT");
                } else if(is_current_word("exit")){
                    tokens[token_counter++] = EXIT;
                    print_to_shell("EXIT STATEMENT");
                } else if(is_current_word("DO")){
                    tokens[token_counter++] = DO;
                    print_to_shell("DO");
                } else if(is_current_word("HOLD")){
                    tokens[token_counter++] = HOLD;
                    print_to_shell("HOLD");
                } else if(is_current_word("INSTANCE")){
                    tokens[token_counter++] = INSTANCE;
                    print_to_shell("INSTANCE");
                } else if(is_digit(ch)){
                    if(ch == '0'){
                        if(peek_next() == 'b'){
                            if(!consume_binary()) {
                                print_error("invalid binary");
                            } else {
                                tokens[token_counter++] = BIN_LITERAL;
                                print_to_shell("BIN LITERAL");
                            }
                        } else if(peek_next() == 'c'){
                            if(!consume_octal()) {
                                print_error("invalid octal");
                            } else {
                                tokens[token_counter++] = OCT_LITERAL;
                                print_to_shell("OCT LITERAL");
                            }
                        } else if(peek_next() == 'x'){
                            if(!consume_hex()) {
                                print_error("invalid hexadecimal");
                            } else {
                                tokens[token_counter++] = HEX_LITERAL;
                                print_to_shell("HEX LITERAL");
                            }
                        }
                    }
                    if(is_digit(ch)){
                        consume_int();
                    }
                }
        }
    } while (sourceCode[i++] != '\0');
}