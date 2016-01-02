#ifndef CLEANTREE_PATH_H_INCLUDED
#define CLEANTREE_PATH_H_INCLUDED


#include <stdbool.h>


struct ct_path {
    char *abs_path;
    bool exists;
    char *given_path;
    bool is_dir;
    char *working_dir;
};


struct ct_path *
ct_path_alloc(char const *working_dir, char const *given_path);

void
ct_path_free(struct ct_path *path);


#endif

