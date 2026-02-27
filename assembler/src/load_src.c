#define LOG_LVL INFO
#include "load_src.h"
#include "util.h"

#include <ctype.h>

vec_impl(Define, Define, define);
vec_impl(char, Char, char);
vec_impl(SrcFile, SrcFile, srcfile);
vec_impl(Line, Line, line);

#define MAX_LINE_LEN 256

char *trim_start(char *str);
char *trim_end(char *str);
char *remove_comment(char *str);
enum Directive parse_directive(char *str, char **arg);
int get_include_path(char *ptr, VecChar *path);
Define parse_define(char *str, size_t file_index, size_t line, const char *line_text);
int apply_defines(VecChar *text, VecDefine *defines);

enum Directive {
    DRV_NOT_DRV,
    DRV_INCLUDE,
    DRV_DEFINE,
};

char *trim_start(char *str) {
    while (*str && isspace(*str)) {
        str++;
    }
    return str;
}

char *trim_end(char *str) {
    char *ptr = str + strlen(str) - 1;
    while (ptr >= str && isspace(*ptr) ) {
        *ptr = 0;
        ptr--;
    }
    return str;
}

char *remove_comment(char *str) {
    char *ptr = strchr(str, ';');
    if (ptr) {
        *ptr = '\0';
    }
    return str;
}

int get_include_path(char *ptr, VecChar *path) {
    reci(*ptr != '"', "extract include path");
    ptr++;

    size_t len = 0;
    while (ptr[len] && ptr[len] != '"') {
        len++;
    }
    reci(ptr[len] != '"', "extract include path");
    vec_char_pusha(path, ptr, len);
    vec_char_push(path, 0);
    return 0;
}

enum Directive parse_directive(char *str, char **arg) {
    if (*str != '%') {
        return DRV_NOT_DRV;
    }
    str++;

    const struct {
        const char *str;
        size_t len;
        enum Directive drv;
    } table[] = {
        { .str = "include", .len = strlen("include"), .drv = DRV_INCLUDE },
        { .str = "define",  .len = strlen("define"),  .drv = DRV_DEFINE  }
    };

    for (size_t i = 0; i< sizeof(table)/sizeof(*table); i++) {
        if (!strncmp(str, table[i].str, table[i].len)) {
            *arg = trim_start(str + table[i].len);
            return table[i].drv;
        }
    }
    return DRV_NOT_DRV;
}

Define parse_define(char *str, size_t file_index, size_t line, const char *line_text) {
    Define def = {
        .name = vec_char_new(),
        .val = vec_char_new(),
        .file_index = file_index,
        .line = line,
        .text = vec_char_new()
    };

    vec_char_pusha(&def.text, line_text, strlen(line_text));
    vec_char_push(&def.text, 0);

    while (*str && !isspace(*str)) {
        vec_char_push(&def.name, *str);
        str++;
    }
    vec_char_push(&def.name, 0);

    str = trim_start(str);
    vec_char_pusha(&def.val, str, strlen(str));
    vec_char_push(&def.val, 0);

    return def;
}

int apply_defines(VecChar *text, VecDefine *defines) {
    for (size_t i = 0; i < vec_define_length(defines); i++) {
        Define *def = vec_define_at(defines, i);
        char *ptr = strstr(text->data, def->name.data);
        if (!ptr) {
            continue;
        }
        char *end_ptr = ptr + vec_char_length(&def->name) - 1;

        int left = ptr == text->data || (!isalnum(*(ptr - 1)) && *(ptr - 1) != '_');
        int right = *end_ptr == '\0' || (!isalnum(*end_ptr) && *end_ptr != '_');
        if (left && right) {
            const char *val = def->val.data;
            size_t val_len = def->val.size - 1;
            size_t name_start = (size_t)(ptr - text->data);
            size_t name_len = vec_char_length(&def->name) - 1;
            vec_char_replace_range(text,
                                   val, val_len,
                                   name_start,
                                   name_len);
            return 1;
        }
    }
    return 0;
}

void recursive_loading(SourceTree *ctx, const char *path);

void recursive_loading(SourceTree *ctx, const char *path) {
    logenter();
    VecChar file_name_vec = vec_char_new();
    vec_char_pusha(&file_name_vec, path, strlen(path) + 1);
    size_t file_index = vec_srcfile_push(&ctx->files, (SrcFile) {
        .name = file_name_vec
    });

    FILE *file = fopen(path, "r+");
    die(!file, "Failed to open file %s", path);
    char buf[MAX_LINE_LEN] = {};
    size_t line_num = 0;
    while (fgets(buf, MAX_LINE_LEN, file)) {
        line_num++;

        char *ptr = trim_end(remove_comment(trim_start(buf)));
        size_t len = strlen(ptr);
        if (!len) {
            continue; // skip empty line
        }

        char *drv_arg = NULL;
        enum Directive drv = parse_directive(ptr, &drv_arg);

        switch (drv) {
        case DRV_INCLUDE:
            VecChar include_path = vec_char_new();
            die(get_include_path(drv_arg, &include_path), "parse include");
            recursive_loading(ctx, include_path.data);
            vec_char_del(&include_path);
            continue;
        break;
        case DRV_DEFINE:
            Define def = parse_define(drv_arg, file_index, line_num, ptr);
            for (size_t i = 0; i < vec_define_length(&ctx->defines); i++) {
                Define *exist_def = vec_define_at(&ctx->defines, i);
                const char *exist_name = exist_def->name.data;
                if (!strcmp(exist_name, def.name.data)) {
                    loge("Redefine \"%s\"\nat:              %s:%zu: %s\nfirst define at: %s:%zu: %s",
                         def.name.data,
                         vec_srcfile_at(&ctx->files, def.file_index)->name.data,
                         def.line,
                         def.text.data,
                         vec_srcfile_at(&ctx->files, exist_def->file_index)->name.data,
                         exist_def->line,
                         exist_def->text.data

                    );
                    die(1, "compile. Fatal error");
                }
            }
            vec_define_push(&ctx->defines, def);
            continue;
        break;
        case DRV_NOT_DRV:
        default:
        }

        VecChar line_text = vec_char_new();
        vec_char_pusha(&line_text, ptr, len + 1);

        while(apply_defines(&line_text, &ctx->defines));

        vec_line_push(&ctx->lines, (Line) {
            .file_index = file_index,
            .number = line_num,
            .text = line_text
        });
    }
}

SourceTree load_src_load(const char *input_file_path) {
    logenter();
    SourceTree src_tree = {
        .files = vec_srcfile_new(),
        .lines = vec_line_new(),
        .defines = vec_define_new()
    };
    recursive_loading(&src_tree, input_file_path);
    return src_tree;
}

void load_src_free(SourceTree *tree) {
    logenter();
    for (size_t i = 0; i < vec_define_length(&tree->defines); i++) {
        vec_char_del(&vec_define_at(&tree->defines, i)->name);
        vec_char_del(&vec_define_at(&tree->defines, i)->val);
        vec_char_del(&vec_define_at(&tree->defines, i)->text);
    }
    for (size_t i = 0; i < vec_line_length(&tree->lines); i++) {
        logd("line: %s:%zu %s",
             vec_srcfile_at(&tree->files, vec_line_at(&tree->lines, i)->file_index)->name.data,
             vec_line_at(&tree->lines, i)->number,
             vec_line_at(&tree->lines, i)->text.data);
        VecChar *line_text = &vec_line_at(&tree->lines, i)->text;
        vec_char_del(line_text);
    }
    for (size_t i = 0; i < vec_srcfile_length(&tree->files); i++) {
        VecChar *name = &vec_srcfile_at(&tree->files, i)->name;
        vec_char_del(name);
    }
    vec_srcfile_del(&tree->files);
    vec_line_del(&tree->lines);
    vec_define_del(&tree->defines);
}
