#ifndef __PARSER_H
#define __PARSER_H

#include <malloc.h>
#include <stdio.h>
#include "tokenizer.h"

struct AST {
    struct Node* head;
};

struct Node {
    Token dest;
    struct Node* next;
};

struct Edge {
    Token src, dest;
};

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

struct AST parse(Token*);

#endif