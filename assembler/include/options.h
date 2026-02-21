#ifndef OPTIONS_H
#define OPTIONS_H

#include "util.h"

#define INPUT_FILES_MAX_COUNT 10

typedef struct {
    char *input_file;
} Options;

int options_parse(int argc, char *argv[], Options *opt);

#endif // OPTIONS_H
