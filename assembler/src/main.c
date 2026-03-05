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

    VecByte output = vec_byte_new();
    reci(asm_assemble(&assembler, &output), "assemble");
    printf("Assembled output (%zu bytes):\n", vec_byte_length(&output));
    for (size_t i = 0; i < vec_byte_length(&output); i++) {
        printf("%02X ", *vec_byte_at(&output, i));
    }
    printf("\n");
    vec_byte_del(&output);

    asm_delete(&assembler);
    tokenizer_delete(&tokenizer);
    load_src_free(&src_tree);
    mem_check_allocated();
    return EXIT_SUCCESS;
}
