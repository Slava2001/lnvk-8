#define LOG_LVL INFO
#include "options.h"

#include <argp.h>

const char *argp_program_version = " version: " TO_STR(TARGET_VERSION);
const char *argp_program_bug_address = "";
static char doc[] = "Assembler";
static char args_doc[] = "FILE...";

static struct argp_option options[] = {
    { 0 }
};

static error_t parse_opt(int key, char *arg, struct argp_state *state);

static struct argp argp = {
    .options = options,
    .parser = parse_opt,
    .args_doc = args_doc,
    .doc = doc
};

int options_parse(int argc, char *argv[], Options *opt) {
    logenter();
    memset(opt, 0, sizeof(Options));
    reci(argp_parse(&argp, argc, argv, 0, 0, opt), "parse options");
    logd("Input file: %s", opt->input_file);
    return 0;
}

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    Options *opt = state->input;

    switch (key) {
    case ARGP_KEY_ARG:
        if (opt->input_file) {
            argp_error(state, "Only one input file supported");
        } else {
            opt->input_file = arg;
        }
        break;
    case ARGP_KEY_END:
        if (!opt->input_file) {
            argp_error(state, "No input files");
        }
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}
