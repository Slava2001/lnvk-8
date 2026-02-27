#ifndef LOAD_SRC_H
#define LOAD_SRC_H

#include "util.h"
#include "vec.h"

typedef struct SrcFile SrcFile;
typedef struct Line Line;
typedef struct Define Define;

vec_decl(char, Char, char);
vec_decl(SrcFile, SrcFile, srcfile);
vec_decl(Line, Line, line);
vec_decl(Define, Define, define);

typedef struct Define {
    VecChar name;
    VecChar val;
    size_t file_index;
    size_t line;
    VecChar text;
} Define;

typedef struct SrcFile {
    VecChar name;
} SrcFile;

typedef struct Line {
    size_t file_index;
    size_t number;
    VecChar text;
} Line;

typedef struct SourceTree {
    VecSrcFile files;
    VecLine lines;
    VecDefine defines;
} SourceTree;

SourceTree load_src_load(const char *input_file_path);
void load_src_free(SourceTree *tree);

#endif // LOAD_SRC_H
