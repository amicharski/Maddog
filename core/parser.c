#include "../include/parser.h"
#include "../include/shell.h"

Token* token_list;
Token token;
int pi = 0;

Token see_next(){
    return token_list[pi+1];
}

void namespace(){

}

void varDecl(){

}

void funcDecl(){

}

void declaration(){
    if(token.token_type == NAMESPACE){
        pi++;
        namespace();
    } else if(token.token_type == DO ||
    token.token_type == HOLD ||
    token.token_type == INSTANCE){
        pi++;
        funcDecl();
    } else if(token.token_type == SIGNED ||
            token.token_type == UNSIGNED ||
            token.token_type == PUBLIC ||
            token.token_type == PRIVATE ||
            token.token_type == PROTECTED){
        pi++;
        varDecl();
    } else {
        print_error("Unexpected token");
    }
}

char* parse(Token* tokens){
    token_list = tokens;
    do {
        token = token_list[pi];
        declaration();
    } while(token.token_type != EOF_TOKEN);
}

