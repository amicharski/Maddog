#ifndef __TOKENIZER_H
#define __TOKENIZER_H

enum TokenType {
    BOOL_LITERAL,
    BIN_LITERAL,
    OCT_LITERAL,
    INT_LITERAL,
    HEX_LITERAL,
    NULL_TOKEN,
    IDENTIFIER,
    IMPORT,
    SIGNED,
    UNSIGNED,
    PUBLIC,
    PRIVATE,
    PROTECTED,
    NAMESPACE,
    RETURN,
    ESCAPE,
    EXIT,
    END_OF_LINE,
    EOF_TOKEN,
    DO,
    HOLD,
    INSTANCE,
    ASSIGN_EQU,
    ASSIGN_ADDEQU,
    ASSIGN_SUBEQU,
    ASSIGN_MULEQU,
    ASSIGN_DIVEQU,
    ASSIGN_EXPEQU,
    ASSIGN_MODEQU,
    ASSIGN_SHIFTRTEQU,
    ASSIGN_SHIFTLTEQU,
    TERNQ,
    IMP,
    OR,
    XOR,
    NOR,
    XNOR,
    AND,
    NAND,
    COND_EQU,
    COND_NOTEQU,
    COND_LTEQU,
    COND_GTEQU,
    SHIFTRT,
    SHIFTLT,
    ADD,
    SUB,
    MULT,
    DIV,
    MOD,
    EXP,
    BANG,
    INCREMENT,
    DECREMENT,
    MULT_TWO,
    DIV_TWO,
    MOD_TWO,
    EXP_TWO,
    DOT,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_ANGLE,
    RIGHT_ANGLE
};

typedef struct {
    enum TokenType token_type;
    char* value;
} Token;

static char* source;
static int token_counter = 0;
char peek_next();
char* peek_range(int);
Token* tokenize(char*);

#endif