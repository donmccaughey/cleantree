#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "src/path.h"
#include "src/path_set.h"


void
test_path_set_alloc(void)
{
    struct ct_path *root_dir = ct_path_alloc(NULL, "/root/dir");
    struct ct_path_set *path_set = ct_path_set_alloc(root_dir);

    assert(path_set);
    assert(0 == path_set->count);
    assert(path_set->paths);
    assert(path_set->root_dir);
    assert(root_dir != path_set->root_dir);
    assert(ct_path_equals_path(root_dir, path_set->root_dir));

    ct_path_set_free(path_set);
    ct_path_free(root_dir);
}


void
test_path_set_add(void)
{
    struct ct_path *root_dir = ct_path_alloc(NULL, "/root/dir");
    struct ct_path_set *path_set = ct_path_set_alloc(root_dir);
    struct ct_path *dir1 = ct_path_alloc(NULL, "/root/dir/dir1");
    struct ct_path *dir2 = ct_path_alloc(NULL, "/tmp");
    struct ct_path *file1 = ct_path_alloc(NULL, "/root/dir/file1");
    struct ct_path *file2 = ct_path_alloc(NULL, "/root/dir/dir1/file2");

    enum ct_path_set_error error;
    int result = ct_path_set_add_path(path_set, dir1, &error);
    assert(0 == result);
    assert(1 == path_set->count);
    assert(ct_path_equals_path(dir1, path_set->paths[0]));

    result = ct_path_set_add_path(path_set, dir2, &error);
    assert(-1 == result);
    assert(ct_path_set_error_not_under_root_dir == error);

    result = ct_path_set_add_path(path_set, file1, &error);
    assert(0 == result);
    assert(2 == path_set->count);
    assert(ct_path_equals_path(dir1, path_set->paths[0]));
    assert(ct_path_equals_path(file1, path_set->paths[1]));

    result = ct_path_set_add_path(path_set, file2, &error);
    assert(0 == result);
    assert(3 == path_set->count);
    assert(ct_path_equals_path(dir1, path_set->paths[0]));
    assert(ct_path_equals_path(file2, path_set->paths[1]));
    assert(ct_path_equals_path(file1, path_set->paths[2]));

    result = ct_path_set_add_path(path_set, root_dir, &error);
    assert(-1 == result);
    assert(ct_path_set_error_path_equals_root_dir == error);

    result = ct_path_set_add_path(path_set, dir1, &error);
    assert(-1 == result);
    assert(ct_path_set_error_duplicate_path == error);
 
    ct_path_free(file2);
    ct_path_free(file1);
    ct_path_free(dir2);
    ct_path_free(dir1);
    ct_path_set_free(path_set);
    ct_path_free(root_dir);
}


int
main(int argc, char *argv[])
{
    test_path_set_alloc();
    test_path_set_add();
    return EXIT_SUCCESS;
}


