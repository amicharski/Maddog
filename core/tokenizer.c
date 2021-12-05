#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/tokenizer.h"
#include "../include/shell.h"

int i = 0;
char range_arr[40];
char* consumed_value;
Token* tokens;
bool debug = false;

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
    int j = 0;
    consumed_value = malloc(32);
    do {
        consumed_value[j] = source[i];
        j++;
    } while(is_bin(source[++i]));
    consumed_value[j] = '\0';
    if(is_alphanumeric(source[--i])) return false;
    return true;
}

bool consume_octal(){
    if(!is_current_word("0c")) return false;
    int j = 0;
    consumed_value = malloc(32);
    do {
        consumed_value[j] = source[i];
        j++;
    } while(is_oct(source[++i]));
    consumed_value[j] = '\0';
    if(is_alphanumeric(source[--i])) return false;
    return true;
}

bool consume_hex(){
    if(!is_current_word("0x")) return false;
    int j = 0;
    consumed_value = malloc(32);
    do {
        consumed_value[j] = source[i];
        j++;
    } while(is_hex(source[++i]));
    consumed_value[j] = '\0';
    if(is_alphanumeric(source[--i])) return false;
    return true;
}

bool consume_int(){
    int j = 0;
    consumed_value = malloc(32);
    do {
        consumed_value[j] = source[i];
        j++;
    } while(is_digit(source[++i]));
    consumed_value[j] = '\0';
    if(is_alpha(source[--i])) return false;
    return true;
}

bool consume_identifier(){
    int j = 0;
    consumed_value = calloc(32, 4);
    do {
        consumed_value[j] = source[i];
        j++;
    } while(is_alphanumeric(source[++i]));
    consumed_value[j] = '\0';
    --i;
    if(debug) printf("\nidentifier: %s\n", consumed_value);
    return true;
}

Token* tokenize(char* sourceCode){
    source = sourceCode;
    tokens = malloc(1024);
    do {
        char ch = sourceCode[i];
        if(debug) printf("\nch: %c\n", ch);
        switch(ch) {
            case '(':
                tokens[token_counter++].token_type = LEFT_PAREN;
                if(debug) print_to_shell("LEFT PARENTHESIS");
                break;
            case ')':
                tokens[token_counter++].token_type = RIGHT_PAREN;
                if(debug) print_to_shell("RIGHT PARENTHESIS");
                break;
            case '[':
                tokens[token_counter++].token_type = LEFT_BRACKET;
                if(debug) print_to_shell("LEFT BRACKET");
                break;
            case ']':
                tokens[token_counter++].token_type = RIGHT_BRACKET;
                if(debug) print_to_shell("RIGHT BRACKET");
                break;
            case '{':
                tokens[token_counter++].token_type = LEFT_BRACE;
                if(debug) print_to_shell("LEFT BRACE");
                break;
            case '}':
                tokens[token_counter++].token_type = RIGHT_BRACE;
                if(debug) print_to_shell("RIGHT BRACE");
                break;
            case '<':
                if (peek_next() == '=') {
                    tokens[token_counter++].token_type = COND_LTEQU;
                    if(debug) print_to_shell("LESS THAN OR EQUAL TO");
                    break;
                } else if(strcmp(peek_range(2), "<=") == 0){
                    tokens[token_counter++].token_type = ASSIGN_SHIFTLTEQU;
                    if(debug) print_to_shell("SHIFT LEFT EQU");
                    break;
                } else if(peek_next() == '<'){
                    tokens[token_counter++].token_type = SHIFTLT;
                    if(debug) print_to_shell("SHIFT LEFT");
                    break;
                } else if(strcmp(peek_range(3), "!+>") == 0){
                    tokens[token_counter++].token_type = XNOR;
                    if(debug) print_to_shell("XNOR");
                    break;
                } else if(strcmp(peek_range(2), "+>") == 0){
                    tokens[token_counter++].token_type = XOR;
                    if(debug) print_to_shell("XOR");
                    break;
                } else {
                    tokens[token_counter++].token_type = LEFT_ANGLE;
                    if(debug) print_to_shell("LEFT ANGLE");
                    break;
                }
            case '>':
                if (peek_next() == '=') {
                    tokens[token_counter++].token_type = COND_GTEQU;
                    if(debug) print_to_shell("GREATER THAN OR EQUAL TO");
                    break;
                } else if(strcmp(peek_range(2), ">=") == 0){
                    tokens[token_counter++].token_type = ASSIGN_SHIFTRTEQU;
                    if(debug) print_to_shell("SHIFT RIGHT EQU");
                    break;
                } else if(peek_next() == '>'){
                    tokens[token_counter++].token_type = SHIFTRT;
                    if(debug) print_to_shell("SHIFT RIGHT");
                    break;
                } else {
                    tokens[token_counter++].token_type = RIGHT_ANGLE;
                    if(debug) print_to_shell("RIGHT ANGLE");
                    break;
                }
            case '.':
                tokens[token_counter++].token_type = DOT;
                if(debug) print_to_shell("DOT");
                break;
            case '&':
                tokens[token_counter++].token_type = AND;
                if(debug) print_to_shell("AND");
                break;
            case '|':
                tokens[token_counter++].token_type = OR;
                if(debug) print_to_shell("OR");
                break;
            case ';':
                tokens[token_counter++].token_type = END_OF_LINE;
                if(debug) print_to_shell("END OF LINE");
                break;
            case '=':
                if(peek_next() == '='){
                    tokens[token_counter++].token_type = COND_EQU;
                    if(debug) print_to_shell("CONDITIONAL EQUAL SIGN");
                    break;
                } else {
                    tokens[token_counter++].token_type = ASSIGN_EQU;
                    if(debug) print_to_shell("ASSIGN EQUAL SIGN");
                    break;
                }
            case '+':
                if(peek_next() == '=') {
                    tokens[token_counter++].token_type = ASSIGN_ADDEQU;
                    if(debug) print_to_shell("ADD EQUAL SIGN");
                    break;
                } else if(peek_next() == '+'){
                    tokens[token_counter++].token_type = INCREMENT;
                    if(debug) print_to_shell("INCREMENT");
                    break;
                } else {
                    tokens[token_counter++].token_type = ADD;
                    if(debug) print_to_shell("ADD");
                    break;
                }
            case '-':
                if(peek_next() == '='){
                    tokens[token_counter++].token_type = ASSIGN_SUBEQU;
                    if(debug) print_to_shell("SUB EQUAL SIGN");
                    break;
                } else if(peek_next() == '-'){
                    tokens[token_counter++].token_type = DECREMENT;
                    if(debug) print_to_shell("DECREMENT");
                    break;
                } else if(peek_next() == '>'){
                    tokens[token_counter++].token_type = IMP;
                    if(debug) print_to_shell("IMPLIES");
                    break;
                } else {
                    tokens[token_counter++].token_type = SUB;
                    if(debug) print_to_shell("SUB");
                    break;
                }
            case '*':
                if(peek_next() == '*'){
                    tokens[token_counter++].token_type = ASSIGN_MULEQU;
                    if(debug) print_to_shell("MULT EQUAL SIGN");
                    break;
                } else if(peek_next() == '*'){
                    tokens[token_counter++].token_type = MULT_TWO;
                    if(debug) print_to_shell("MULT TWO");
                    break;
                } else {
                    tokens[token_counter++].token_type = MULT;
                    if(debug) print_to_shell("MULT");
                    break;
                }
            case '/':
                if(peek_next() == '='){
                    tokens[token_counter++].token_type = ASSIGN_DIVEQU;
                    if(debug) print_to_shell("DIV EQUAL SIGN");
                    break;
                } else if(peek_next() == '/'){
                    tokens[token_counter++].token_type = DIV_TWO;
                    if(debug) print_to_shell("DIV TWO");
                    break;
                } else {
                    tokens[token_counter++].token_type = DIV;
                    if(debug) print_to_shell("DIV");
                    break;
                }
            case '%':
                if(peek_next() == '='){
                    tokens[token_counter++].token_type = ASSIGN_MODEQU;
                    if(debug) print_to_shell("MOD EQUAL SIGN");
                    break;
                } else if(peek_next() == '%'){
                    tokens[token_counter++].token_type = MOD_TWO;
                    if(debug) print_to_shell("MOD TWO");
                    break;
                } else {
                    tokens[token_counter++].token_type = MOD;
                    if(debug) print_to_shell("MOD");
                    break;
                }
            case '^':
                if(peek_next() == '='){
                    tokens[token_counter++].token_type = ASSIGN_EXPEQU;
                    if(debug) print_to_shell("EXP EQUAL SIGN");
                    break;
                } else if(peek_next() == '^'){
                    tokens[token_counter++].token_type = EXP_TWO;
                    if(debug) print_to_shell("EXP TWO");
                    break;
                } else {
                    tokens[token_counter++].token_type = EXP;
                    if(debug) print_to_shell("EXP");
                    break;
                }
            case '?':
                tokens[token_counter++].token_type = TERNQ;
                if(debug) print_to_shell("TERNARY QUESTION MARK");
                break;
            case '!':
                if(peek_next() == '='){
                    tokens[token_counter++].token_type = COND_NOTEQU;
                    if(debug) print_to_shell("NOT EQUAL");
                    break;
                } else if(peek_next() == '|'){
                    tokens[token_counter++].token_type = NOR;
                    if(debug) print_to_shell("NOR");
                    break;
                } else if(peek_next() == '&'){
                    tokens[token_counter++].token_type = NAND;
                    if(debug) print_to_shell("NAND");
                    break;
                } else {
                    tokens[token_counter++].token_type = BANG;
                    if(debug) print_to_shell("BANG");
                    break;
                }
            default:
                if(is_current_word("import")){
                    tokens[token_counter++].token_type = IMPORT;
                    if(debug) print_to_shell("IMPORT STATEMENT");
                } else if(is_current_word("signed")){
                    tokens[token_counter++].token_type = SIGNED;
                    if(debug) print_to_shell("SIGNED");
                } else if(is_current_word("unsigned")){
                    tokens[token_counter++].token_type = UNSIGNED;
                    if(debug) print_to_shell("UNSIGNED");
                } else if(is_current_word("public")){
                    tokens[token_counter++].token_type = PUBLIC;
                    if(debug) print_to_shell("PUBLIC");
                } else if(is_current_word("private")){
                    tokens[token_counter++].token_type = PRIVATE;
                    if(debug) print_to_shell("PRIVATE");
                } else if(is_current_word("protected")){
                    tokens[token_counter++].token_type = PROTECTED;
                    if(debug) print_to_shell("PROTECTED");
                } else if(is_current_word("namespace")){
                    tokens[token_counter++].token_type = NAMESPACE;
                    if(debug) print_to_shell("NAMESPACE DECLARATION");
                } else if(is_current_word("return")){
                    tokens[token_counter++].token_type = RETURN;
                    if(debug) print_to_shell("RETURN STATEMENT");
                } else if(is_current_word("esc")){
                    tokens[token_counter++].token_type = ESCAPE;
                    if(debug) print_to_shell("ESCAPE STATEMENT");
                } else if(is_current_word("exit")){
                    tokens[token_counter++].token_type = EXIT;
                    if(debug) print_to_shell("EXIT STATEMENT");
                } else if(is_current_word("DO")){
                    tokens[token_counter++].token_type = DO;
                    if(debug) print_to_shell("DO");
                } else if(is_current_word("HOLD")){
                    tokens[token_counter++].token_type = HOLD;
                    if(debug) print_to_shell("HOLD");
                } else if(is_current_word("INSTANCE")){
                    tokens[token_counter++].token_type = INSTANCE;
                    if(debug) print_to_shell("INSTANCE");
                } else if(is_current_word("TRUE")) {
                    tokens[token_counter].token_type = BOOL_LITERAL;
                    tokens[token_counter++].value = "TRUE";
                    if(debug) print_to_shell("BOOL LITERAL");
                } else if(is_current_word("FALSE")) {
                    tokens[token_counter].token_type = BOOL_LITERAL;
                    tokens[token_counter++].value = "FALSE";
                    if(debug) print_to_shell("BOOL LITERAL");
                } else if(is_current_word("NULL")){
                    tokens[token_counter++].token_type = NULL_TOKEN;
                    if(debug) print_to_shell("NULL TOKEN");
                } else if(is_digit(ch)){
                    if(ch == '0'){
                        if(peek_next() == 'b'){
                            if(!consume_binary()) {
                                print_error("invalid binary");
                            } else {
                                tokens[token_counter].token_type = BIN_LITERAL;
                                tokens[token_counter++].value = consumed_value;
                                if(debug) print_to_shell("BIN LITERAL");
                                continue;
                            }
                        } else if(peek_next() == 'c'){
                            if(!consume_octal()) {
                                print_error("invalid octal");
                            } else {
                                tokens[token_counter].token_type = OCT_LITERAL;
                                tokens[token_counter++].value = consumed_value;
                                if(debug) print_to_shell("OCT LITERAL");
                                continue;
                            }
                        } else if(peek_next() == 'x'){
                            if(!consume_hex()) {
                                print_error("invalid hexadecimal");
                            } else {
                                tokens[token_counter++].token_type = HEX_LITERAL;
                                tokens[token_counter++].value = consumed_value;
                                if(debug) print_to_shell("HEX LITERAL");
                                continue;
                            }
                        }
                    }
                    if(consume_int()){
                        tokens[token_counter++].token_type = INT_LITERAL;
                        tokens[token_counter++].value = consumed_value;
                        if(debug) print_to_shell("INT LITERAL");
                        continue;
                    }
                } else if(is_alpha(ch)){
                    consume_identifier();
                    tokens[token_counter++].token_type = IDENTIFIER;
                    tokens[token_counter++].value = consumed_value;
                    if(debug) print_to_shell("IDENTIFIER");
                    continue;
                }
        }
    } while (sourceCode[i++] != '\0');
    tokens[token_counter++].token_type = EOF_TOKEN;
    tokens[token_counter++].value = consumed_value;
    if(debug) print_to_shell("EOF");
    return tokens;
}