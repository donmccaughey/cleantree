#include "path.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>


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

    if ('/' == given_path[0]) {
        path->abs_path = strdup(given_path);
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

