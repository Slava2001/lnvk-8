#define LOG_LVL INFO
#include "util.h"
#include "options.h"
#include "load_src.h"

int main(int argc, char *argv[]) {
    logenter();
    Options opt = {};
    reci(options_parse(argc, argv, &opt), "parse arguments");
    SourceTree src_tree = load_src_load(opt.input_file);

    load_src_free(&src_tree);
    mem_check_allocated();
    return EXIT_SUCCESS;
}
