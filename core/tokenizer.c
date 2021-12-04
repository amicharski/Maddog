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
    i += strlen(word);
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
    if(is_alphanumeric(source[--i])) return false;
    return true;
}

bool consume_octal(){
    if(!is_current_word("0c")) return false;
    do {} while(is_oct(source[i++]));
    if(is_alphanumeric(source[--i])) return false;
    return true;
}

bool consume_hex(){
    if(!is_current_word("0x")) return false;
    do {} while(is_hex(source[i++]));
    if(is_alphanumeric(source[--i])) return false;
    return true;
}

bool consume_int(){
    do {} while(is_digit(source[i++]));
    if(is_alpha(source[--i])) return false;
    return true;
}

bool consume_identifier(){
    do {} while(is_alphanumeric(source[i++]));
    --i;
    return true;
}

char* tokenize(char* sourceCode){
    source = sourceCode;
    tokens = malloc(1024);
    do {
        char ch = sourceCode[i];
        printf("\nch: %c\n", ch);
        switch(ch) {
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
                if (peek_next() == '=') {
                    tokens[token_counter++] = COND_LTEQU;
                    print_to_shell("LESS THAN OR EQUAL TO");
                    break;
                } else if(strcmp(peek_range(2), "<=") == 0){
                    tokens[token_counter++] = ASSIGN_SHIFTLTEQU;
                    print_to_shell("SHIFT LEFT EQU");
                    break;
                } else if(peek_next() == '<'){
                    tokens[token_counter++] = SHIFTLT;
                    print_to_shell("SHIFT LEFT");
                    break;
                } else if(strcmp(peek_range(3), "!+>") == 0){
                    tokens[token_counter++] = XNOR;
                    print_to_shell("XNOR");
                    break;
                } else if(strcmp(peek_range(2), "+>") == 0){
                    tokens[token_counter++] = XOR;
                    print_to_shell("XOR");
                    break;
                } else {
                    tokens[token_counter++] = LEFT_ANGLE;
                    print_to_shell("LEFT ANGLE");
                    break;
                }
            case '>':
                if (peek_next() == '=') {
                    tokens[token_counter++] = COND_GTEQU;
                    print_to_shell("GREATER THAN OR EQUAL TO");
                    break;
                } else if(strcmp(peek_range(2), ">=") == 0){
                    tokens[token_counter++] = ASSIGN_SHIFTRTEQU;
                    print_to_shell("SHIFT RIGHT EQU");
                    break;
                } else if(peek_next() == '>'){
                    tokens[token_counter++] = SHIFTRT;
                    print_to_shell("SHIFT RIGHT");
                    break;
                } else {
                    tokens[token_counter++] = RIGHT_ANGLE;
                    print_to_shell("RIGHT ANGLE");
                    break;
                }
            case '.':
                tokens[token_counter++] = DOT;
                print_to_shell("DOT");
                break;
            case ';':
                tokens[token_counter++] = END_OF_LINE;
                print_to_shell("END OF LINE");
                break;
            case '=':
                if(peek_next() == '='){
                    tokens[token_counter++] = COND_EQU;
                    print_to_shell("CONDITIONAL EQUAL SIGN");
                    break;
                } else {
                    tokens[token_counter++] = ASSIGN_EQU;
                    print_to_shell("ASSIGN EQUAL SIGN");
                    break;
                }
            case '+':
                if(peek_next() == '=') {
                    tokens[token_counter++] = ASSIGN_ADDEQU;
                    print_to_shell("ADD EQUAL SIGN");
                    break;
                } else if(peek_next() == '+'){
                    tokens[token_counter++] = INCREMENT;
                    print_to_shell("INCREMENT");
                    break;
                } else {
                    tokens[token_counter++] = ADD;
                    print_to_shell("ADD");
                    break;
                }
            case '-':
                if(peek_next() == '='){
                    tokens[token_counter++] = ASSIGN_SUBEQU;
                    print_to_shell("SUB EQUAL SIGN");
                    break;
                } else if(peek_next() == '-'){
                    tokens[token_counter++] = DECREMENT;
                    print_to_shell("DECREMENT");
                    break;
                } else {
                    tokens[token_counter++] = SUB;
                    print_to_shell("SUB");
                    break;
                }
            case '*':
                if(peek_next() == '*'){
                    tokens[token_counter++] = ASSIGN_MULEQU;
                    print_to_shell("MULT EQUAL SIGN");
                    break;
                } else if(peek_next() == '*'){
                    tokens[token_counter++] = MULT_TWO;
                    print_to_shell("MULT TWO");
                    break;
                } else {
                    tokens[token_counter++] = MULT;
                    print_to_shell("MULT");
                    break;
                }
            case '/':
                if(peek_next() == '='){
                    tokens[token_counter++] = ASSIGN_DIVEQU;
                    print_to_shell("DIV EQUAL SIGN");
                    break;
                } else if(peek_next() == '/'){
                    tokens[token_counter++] = DIV_TWO;
                    print_to_shell("DIV TWO");]
                    break;
                } else {
                    tokens[token_counter++] = DIV;
                    print_to_shell("DIV");
                    break;
                }
            case '%':
                if(peek_next() == '='){
                    tokens[token_counter++] = ASSIGN_MODEQU;
                    print_to_shell("MOD EQUAL SIGN");
                    break;
                } else if(peek_next() == '%'){
                    tokens[token_counter++] = MOD_TWO;
                    print_to_shell("MOD TWO");
                    break;
                } else {
                    tokens[token_counter++] = MOD;
                    print_to_shell("MOD");
                    break;
                }
            case '^':
                if(peek_next() == '='){
                    tokens[token_counter++] = ASSIGN_EXPEQU;
                    print_to_shell("EXP EQUAL SIGN");
                    break;
                } else if(peek_next() == '^'){
                    tokens[token_counter++] = EXP_TWO;
                    print_to_shell("EXP TWO");
                    break;
                } else {
                    tokens[token_counter++] = EXP;
                    print_to_shell("EXP");
                    break;
                }
            case '?':
                tokens[token_counter++] = TERNQ;
                print_to_shell("TERNARY QUESTION MARK");
                break;
            case '!':

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
                } else if(is_current_word("TRUE")
                || is_current_word("FALSE")) {
                    tokens[token_counter++] = BOOL_LITERAL;
                    print_to_shell("BOOL LITERAL");
                } else if(is_current_word("NULL")){
                    tokens[token_counter++] = NULL_TOKEN;
                    print_to_shell("NULL TOKEN");
                } else if(is_digit(ch)){
                    if(ch == '0'){
                        if(peek_next() == 'b'){
                            if(!consume_binary()) {
                                print_error("invalid binary");
                            } else {
                                tokens[token_counter++] = BIN_LITERAL;
                                print_to_shell("BIN LITERAL");
                                continue;
                            }
                        } else if(peek_next() == 'c'){
                            if(!consume_octal()) {
                                print_error("invalid octal");
                            } else {
                                tokens[token_counter++] = OCT_LITERAL;
                                print_to_shell("OCT LITERAL");
                                continue;
                            }
                        } else if(peek_next() == 'x'){
                            if(!consume_hex()) {
                                print_error("invalid hexadecimal");
                            } else {
                                tokens[token_counter++] = HEX_LITERAL;
                                print_to_shell("HEX LITERAL");
                                continue;
                            }
                        }
                    }
                    if(consume_int()){
                        tokens[token_counter++] = INT_LITERAL;
                        print_to_shell("INT LITERAL");
                        continue;
                    }
                } else if(is_alpha(ch)){
                    consume_identifier();
                    tokens[token_counter++] = IDENTIFIER;
                    print_to_shell("IDENTIFIER");
                    continue;
                }
        }
    } while (sourceCode[i++] != '\0');
    tokens[token_counter++] = EOF_TOKEN;
    print_to_shell("EOF");
}