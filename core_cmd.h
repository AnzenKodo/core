#ifndef LIBS_ARGS
#define LIBS_ARGS

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

size_t arg_c = 0;
char **arg_v;

typedef struct {
    const char *name;
    const char *shortname;
    const char *desc;
} Args_List;

#define ARGS_LIST_SIZE 10
Args_List args_list[ARGS_LIST_SIZE];
size_t args_list_count = 0;

void args_list_add(const char *name, const char *shortname, const char *desc) {
    if (args_list_count < ARGS_LIST_SIZE) {
        args_list[args_list_count].name = name;
        args_list[args_list_count].shortname = shortname;
        args_list[args_list_count].desc = desc;
        args_list_count++;
    }
}

char *args_exist(char const *name, char jump) {
    for (size_t i = 0; i < arg_c; i++) {
        char *arg_ptr = arg_v[i];
        char *name_ptr = (char *)name;

        while(*arg_ptr && (*arg_ptr == *name_ptr)) {
            arg_ptr++, name_ptr++;
        }

        if ((*(unsigned char *)arg_ptr - *(unsigned char *)name_ptr) == 0) {
            char *val = arg_v[i + jump];

            if (!val) {
                fprintf(stdout, "Error: no value is provided after argument `%s`.\n", name);
                exit(1);
            }

            return val;
        }
    }

    return NULL;
}

void args_init(size_t argc, char *argv[]) {
    arg_c = argc;
    arg_v = argv;
}

char *args_get(const char *name, const char *shortname, const char *desc) {
    args_list_add(name, shortname, desc);

    char *val = args_exist(name, 1);
    if (!val) {
        val = args_exist(shortname, 1);
    }
    return val;
}

int args_get_int(const char *name, const char *shortname, const char *desc) {
    args_list_add(name, shortname, desc);

    char *val = args_exist(name, 1);
    if (!val) val = args_exist(shortname, 1);
    if (!val) return -1;

    int val_num = atoi(val);

    if ((strcmp(val, "0") != 0) && val_num == 0) {
        fprintf(stdout, "Error: argument `%s` only support integer values. Given value: %s\n", name, val);
        exit(1);
    }

    return val_num;
}

float args_get_float(const char *name, const char *shortname, const char *desc) {
    args_list_add(name, shortname, desc);

    char *val = args_exist(name, 1);
    if (!val) val = args_exist(shortname, 1);
    if (!val) return -1;

    float val_num = atof(val);

    if ((strcmp(val, "0") != 0) && val_num == 0) {
        fprintf(stdout, "Error: argument `%s` only support integer values. Given value: %s\n", name, val);
        exit(1);
    }

    return val_num;
}

bool args_has(const char *name, const char *shortname, const char *desc) {
    args_list_add(name, shortname, desc);

    if (args_exist(name, 0)) return true;
    if (args_exist(shortname, 0)) return true;

    return false;
}

void args_print_help(char *name, char *desc, char *format, char *footer) {
    printf("%s: %s\n", name, desc);
    printf("Usage:\n    %s %s\n",name, format);
    printf("Options:\n");
    for (size_t i = 0; i < args_list_count; i++) {
        printf("    %-10s %-3s %s\n", args_list[i].name, args_list[i].shortname, args_list[i].desc);
    }
    printf(footer);
    puts("");
}

#endif // LIBS_ARGS
