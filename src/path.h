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

struct ct_path *
ct_path_alloc_copy(struct ct_path const *path);

void
ct_path_free(struct ct_path *path);

int
ct_path_compare_path(struct ct_path const *path,
                     struct ct_path const *other);

bool
ct_path_equals_path(struct ct_path const *path,
                    struct ct_path const *other);

bool
ct_path_is_under_path(struct ct_path const *path, 
                      struct ct_path const *parent);


#endif

