#define LOG_LVL INFO
#include "util.h"
#include "options.h"
#include "load_src.h"
#include "tokenizer.h"

int main(int argc, char *argv[]) {
    logenter();
    Options opt = {};
    reci(options_parse(argc, argv, &opt), "parse arguments");
    SourceTree src_tree = load_src_load(opt.input_file);
    Tokenizer tokenizer = tokenizer_new(&src_tree);
    while (1) {
        Token tok = tokenizer_next(&tokenizer);
        if (tok.type == TOK_EOF) {
            break;
        }
        logi("Tok: %d, \"%.*s\"", tok.type, (int)tok.len, tok.word);
    }
    tokenizer_delete(&tokenizer);
    load_src_free(&src_tree);
    mem_check_allocated();
    return EXIT_SUCCESS;
}
