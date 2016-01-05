#include "path_set.h"

#include <errno.h>
#include <stdlib.h>

#include "path.h"

#include <stdio.h>
int
ct_path_set_add_path(struct ct_path_set *path_set,
                     struct ct_path const *path,
                     enum ct_path_set_error *error)
{
    if (!error) {
        errno = EINVAL;
        return -1;
    }
    if (!path_set || !path) {
        errno = EINVAL;
        *error = ct_path_set_error_errno;
        return -1;
    }
    if (!ct_path_is_under_path(path, path_set->root_dir)) {
        *error = ct_path_set_error_not_under_root_dir;
        return -1;
    }

    int new_index = path_set->count;
    int new_count = path_set->count + 1;
    size_t new_size = new_count * sizeof(struct ct_path *);
    struct ct_path **new_paths = realloc(path_set->paths, new_size);
    if (!new_paths) {
        *error = ct_path_set_error_errno;
        return -1;
    }

    path_set->paths = new_paths;
    path_set->paths[new_index] = ct_path_alloc_copy(path);
    if (!path_set->paths[new_index]) {
        *error = ct_path_set_error_errno;
        return -1;
    }
    path_set->count = new_count;

    return 0;
}


struct ct_path_set *
ct_path_set_alloc(struct ct_path const *root_dir)
{
    struct ct_path_set *path_set = calloc(1, sizeof(struct ct_path_set));
    if (!path_set) return NULL;
    
    path_set->root_dir = ct_path_alloc_copy(root_dir);
    if (!path_set->root_dir) {
        ct_path_set_free(path_set);
        return NULL;
    }

    path_set->paths = calloc(1, sizeof(struct ct_path_set *));
    if (!path_set->paths) {
        ct_path_set_free(path_set);
        return NULL;
    }

    return path_set;
}


void
ct_path_set_free(struct ct_path_set *path_set)
{
    if (path_set) {
        ct_path_free(path_set->root_dir);
        for (int i = 0; i < path_set->count; ++i) {
            ct_path_free(path_set->paths[i]);
        }
        free(path_set->paths);
    }
}

