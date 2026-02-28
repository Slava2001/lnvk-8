#define LOG_LVL INFO
#include "util.h"
#include "options.h"
#include "load_src.h"
#include "tokenizer.h"
#include "asm.h"

int main(int argc, char *argv[]) {
    logenter();
    Options opt = {};
    reci(options_parse(argc, argv, &opt), "parse arguments");
    SourceTree src_tree = load_src_load(opt.input_file);
    Tokenizer tokenizer = tokenizer_new(&src_tree);
    Asm assembler = asm_new(&tokenizer);

    uint8_t output[65536];
    ssize_t output_len = asm_assemble(&assembler, output, sizeof(output));
    if (output_len < 0) {
        loge("Failed to assemble");
        return EXIT_FAILURE;
    }

    asm_delete(&assembler);
    tokenizer_delete(&tokenizer);
    load_src_free(&src_tree);
    mem_check_allocated();
    return EXIT_SUCCESS;
}
