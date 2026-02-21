#ifndef AST_H
#define AST_H

#include "load_src.h"

enum Token {
    TOK_KW_MOV,
    TOK_RG_0
}

typedef struct AST {

} AST;

AST ast_build(const SrcFile *src);

#endif // AST_H
