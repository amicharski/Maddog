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

struct AST* initialize_ast();

void print_ast(struct AST* ast);

struct AST* parse(Token*);

#endif