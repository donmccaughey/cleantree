#include "path_set.h"

#include <stdlib.h>

#include "path.h"


struct ct_path_set *
ct_path_set_alloc(struct ct_path const *root_dir)
{
    struct ct_path_set *path_set = calloc(1, sizeof(struct ct_path_set));
    if (!path_set) return NULL;

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

