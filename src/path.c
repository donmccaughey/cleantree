#include "path.h"

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>


inline static bool
is_abs(char const *path)
{
    return '/' == path[0];
}


inline static bool
is_root(char const *path)
{
    return is_abs(path) && '\0' == path[1];
}


struct ct_path *
ct_path_alloc(char const *working_dir, char const *given_path)
{
    if (!given_path || !given_path[0]) {
        errno = EINVAL;
        return NULL;
    }

    struct ct_path *path = calloc(1, sizeof(struct ct_path));
    if (!path) return NULL;

    path->working_dir = working_dir ? strdup(working_dir) : getcwd(NULL, 0);
    if (!path->working_dir) {
        ct_path_free(path);
        return NULL;
    }

    path->given_path = strdup(given_path);
    if (!path->given_path) {
        ct_path_free(path);
        return NULL;
    }

    if (is_abs(given_path)) {
        path->abs_path = strdup(given_path);
    } else if (is_root(path->working_dir)) {
        asprintf(&path->abs_path, "/%s", path->given_path);
    } else {
        asprintf(&path->abs_path, "%s/%s", path->working_dir, path->given_path);
    }
    if (!path->abs_path) {
        ct_path_free(path);
        return NULL;
    }

    struct stat status;
    int result = stat(path->abs_path, &status);
    if (-1 == result) {
        if (ENOENT != errno) {
            ct_path_free(path);
            return NULL;
        } else {
            path->exists = false;
        }
    } else {
        path->exists = true;
        if (S_IFDIR & status.st_mode) path->is_dir = true;
    }

    return path;
}


struct ct_path *
ct_path_alloc_copy(struct ct_path const *path)
{
    if (!path) {
        errno = EINVAL;
        return NULL;
    }

    struct ct_path *copy = malloc(sizeof(struct ct_path));
    if (!copy) return NULL;

    copy->abs_path = strdup(path->abs_path);
    if (!copy->abs_path) {
        ct_path_free(copy);
        return NULL;
    }
    copy->exists = path->exists;
    copy->given_path = strdup(path->given_path);
    if (!copy->given_path) {
        ct_path_free(copy);
        return NULL;
    }
    copy->is_dir = path->is_dir;
    copy->working_dir = strdup(path->working_dir);
    if (!copy->working_dir) {
        ct_path_free(copy);
        return NULL;
    }

    return copy;
}


struct ct_path *
ct_path_alloc_dir(struct ct_path const *path)
{
    if (!path) {
        errno = EINVAL;
        return NULL;
    }

    char *abs_path = strdup(path->abs_path);
    if (!abs_path) return NULL;

    struct ct_path *dir_path = NULL;
    char *dir = dirname(abs_path);
    if (dir) dir_path = ct_path_alloc(NULL, dir);

    free(abs_path);
    return dir_path;
}


int
ct_path_compare_path(struct ct_path const *path,
                     struct ct_path const *other)
{
    if (!path || !other) return -1;

    return strcmp(path->abs_path, other->abs_path);
}


void
ct_path_free(struct ct_path *path)
{
    if (path) {
        free(path->abs_path);
        free(path->given_path);
        free(path->working_dir);
        free(path);
    }
}


bool
ct_path_equals_path(struct ct_path const *path,
                    struct ct_path const *other)
{
    if (!path || !other) return false;

    return 0 == strcmp(path->abs_path, other->abs_path);
}


bool
ct_path_is_under_path(struct ct_path const *path, 
                      struct ct_path const *parent)
{
    if (!path || !parent) return false;

    size_t parent_length = strlen(parent->abs_path);
    size_t path_length = strlen(path->abs_path);    

    if (path_length < parent_length) return false;
    if (0 != strncmp(parent->abs_path, path->abs_path, parent_length)) return false;
    if (path_length == parent_length) return false;
    return '/' == path->abs_path[parent_length];
}

