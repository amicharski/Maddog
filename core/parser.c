#include <stdbool.h>
#include "../include/shell.h"
#include "../include/parser.h"

Token* token_list;
Token token;
struct AST *parse_tree;
int imps = 0;
int pi = 0;

struct AST* initialize_ast(){
    struct AST* ast = (struct AST*)malloc(sizeof(struct AST));
    return ast;
}

void print_ast(struct AST* ast){
    for(int i = 0; i < sizeof(ast->head); i++){
        struct Node* ptr = &ast->head[i];
        while(ptr != NULL){
            printf("(%d -> %d\t", i, ptr->dest);
            ptr = ptr->next;
        }

        printf("\n");
    }
}

Token tpeek_next(){
    return token_list[pi+1];
}

Token tget_token(){
    return token_list[pi];
}

void advance(){
    pi++;
}

bool token_is(enum TokenType tkn){
    return tget_token().token_type == tkn;
}

void ref(){

}

void refs(){

}

void import(struct Node *node){
    advance();

}

void imports(struct Node *node){
    if(token_is(IMPORT)){
        (*node).next[imps++].dest = tget_token();
        import(node);
    }
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

struct AST* parse(Token *tokens){
    printf("parse() started\n");
    return 'a';
    initialize_ast();

    token_list = tokens;
    Token start_tkn;
    start_tkn.token_type = START;

    struct Node start_node;
    start_node.dest = start_tkn;

    printf("oh we're halfway there\n");

    imports(&start_node);
    printf("woman i am willing\n");
    print_ast(parse_tree);

    printf("as soon as you are able\n");

    return parse_tree;
//    do {
//        token = token_list[pi];
//        imports();
//    } while(token.token_type != EOF_TOKEN);
}

