#include "path_set.h"

#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "path.h"


static int
compare_paths(void const *first, void const *second)
{
    struct ct_path const *const *path1 = first;
    struct ct_path const *const *path2 = second;
    return ct_path_compare_path(*path1, *path2);
}


static int
find_all_children(struct ct_path_set *path_set, struct ct_path const *path)
{
    DIR *dir = opendir(path->abs_path);
    if (!dir) return -1;

    struct dirent *entry;
    errno = 0;
    while ((entry = readdir(dir))) {
        if (0 == strcmp(".", entry->d_name)) continue;
        if (0 == strcmp("..", entry->d_name)) continue;

        struct ct_path *child = ct_path_alloc(path->abs_path, entry->d_name);
        if (!child) {
            closedir(dir);
            return -1;
        }

        enum ct_path_set_error error;
        int result = ct_path_set_add_path(path_set, child, &error);
        if (-1 == result) {
            ct_path_free(child);
            closedir(dir);
            return -1;
        }

        if (DT_DIR == entry->d_type) {
            int result = find_all_children(path_set, child);
            if (-1 == result) {
                ct_path_free(child);
                closedir(dir);
                return -1;
            }
        } else if (DT_LNK == entry->d_type) {
            // TODO: handle symlinks
            ct_path_free(child);
            closedir(dir);
            return -1;
        }

        ct_path_free(child);
    }
    closedir(dir);
    return 0;
}


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
    if (ct_path_equals_path(path, path_set->root_dir)) {
        *error = ct_path_set_error_path_equals_root_dir;
        return -1;
    }
    if (!ct_path_is_under_path(path, path_set->root_dir)) {
        *error = ct_path_set_error_not_under_root_dir;
        return -1;
    }
    if (path_set->count && ct_path_set_contains_path(path_set, path)) {
        *error = ct_path_set_error_duplicate_path;
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

    if (path_set->count > 1) {
        qsort(path_set->paths, path_set->count, sizeof(path_set->paths[0]),
              compare_paths);
    }

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


struct ct_path_set *
ct_path_set_alloc_difference(struct ct_path_set const *path_set,
                             struct ct_path_set const *path_set_to_remove)
{
    struct ct_path_set *difference = ct_path_set_alloc(path_set->root_dir);
    if (!difference) return NULL;
    
    for (int i = 0; i < path_set->count; ++i) {
        if (!ct_path_set_contains_path(path_set_to_remove, path_set->paths[i])) {
            enum ct_path_set_error error;
            int result = ct_path_set_add_path(difference, path_set->paths[i], &error);
            if (-1 == result) {
                ct_path_set_free(difference);
                return NULL;
            }
        }
    }
    return difference;
}


bool
ct_path_set_contains_path(struct ct_path_set const *path_set,
                          struct ct_path const *path)
{
    void *found_path = bsearch(&path, path_set->paths, path_set->count, 
                               sizeof(path_set->paths[0]), compare_paths);
    return NULL != found_path;
}


int
ct_path_set_find_all(struct ct_path_set *path_set)
{
    if (!path_set) {
        errno = EINVAL;
        return -1;
    }
    return find_all_children(path_set, path_set->root_dir);
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

