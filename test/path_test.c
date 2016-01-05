#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "src/path.h"


void
test_path_alloc(void)
{
    char *working_dir = getcwd(NULL, 0);
    size_t working_dir_len = strlen(working_dir);

    struct ct_path *path = ct_path_alloc(NULL, "my/path");

    assert(path);

    assert(path->abs_path);
    assert(0 == strncmp(working_dir, path->abs_path, working_dir_len));
    size_t path_sep_index = working_dir_len;
    assert('/' == path->abs_path[path_sep_index]);
    size_t given_path_start = path_sep_index + 1;
    assert(0 == strcmp("my/path", &path->abs_path[given_path_start]));

    assert(path->given_path);
    assert(0 == strcmp("my/path", path->given_path));
    
    assert(path->working_dir);
    assert(0 == strcmp(working_dir, path->working_dir));

    free(working_dir);
    ct_path_free(path);
}


void
test_path_alloc_copy(void)
{
    struct ct_path *path = ct_path_alloc(NULL, "my/path");
    struct ct_path *copy = ct_path_alloc_copy(path);

    assert(copy);

    assert(copy->abs_path);
    assert(0 == strcmp(path->abs_path, copy->abs_path));

    assert(path->exists == copy->exists);

    assert(copy->given_path);
    assert(0 == strcmp(path->given_path, copy->given_path));

    assert(path->is_dir == copy->is_dir);

    assert(copy->working_dir);
    assert(0 == strcmp(path->working_dir, copy->working_dir));

    ct_path_free(copy);
    ct_path_free(path);
}


void
test_path_alloc_with_abs_given_path(void)
{
    struct ct_path *path = ct_path_alloc("/current/working/dir", "/my/path");

    assert(path);
    
    assert(path->abs_path);
    assert(0 == strcmp("/my/path", path->abs_path));

    assert(path->given_path);
    assert(0 == strcmp("/my/path", path->given_path));

    assert(path->working_dir);
    assert(0 == strcmp("/current/working/dir", path->working_dir));

    ct_path_free(path);
}


void
test_path_alloc_with_relative_given_path(void)
{
    struct ct_path *path = ct_path_alloc("/current/working/dir", "my/path");

    assert(path);

    assert(path->abs_path);
    assert(0 == strcmp("/current/working/dir/my/path", path->abs_path));

    assert(path->given_path);
    assert(0 == strcmp("my/path", path->given_path));
    
    assert(path->working_dir);
    assert(0 == strcmp("/current/working/dir", path->working_dir));

    ct_path_free(path);
}


void
test_path_alloc_with_root_working_dir(void)
{
    struct ct_path *path = ct_path_alloc("/", "my/path");

    assert(path);

    assert(path->abs_path);
    assert(0 == strcmp("/my/path", path->abs_path));

    assert(path->given_path);
    assert(0 == strcmp("my/path", path->given_path));
    
    assert(path->working_dir);
    assert(0 == strcmp("/", path->working_dir));

    ct_path_free(path);
}


void
test_path_equals_path(void)
{
    struct ct_path *path1 = ct_path_alloc("/tmp", "foo");
    struct ct_path *path2 = ct_path_alloc("/var", "/tmp/foo");
    struct ct_path *path3 = ct_path_alloc("/tmp", "bar");

    assert(ct_path_equals_path(path1, path1));
    assert(ct_path_equals_path(path2, path2));

    assert(ct_path_equals_path(path1, path2));
    assert(ct_path_equals_path(path2, path1));

    assert(!ct_path_equals_path(path1, path3));
    assert(!ct_path_equals_path(path3, path1));

    ct_path_free(path3);
    ct_path_free(path2);
    ct_path_free(path1);
}


void
test_path_is_under_path(void)
{
    struct ct_path *root = ct_path_alloc(NULL, "/my/root");
    struct ct_path *child = ct_path_alloc(NULL, "/my/root/some/child");
    struct ct_path *other = ct_path_alloc(NULL, "/some/other/path"); 
    struct ct_path *relative_root = ct_path_alloc(NULL, "my_root");
    struct ct_path *relative_child = ct_path_alloc(NULL, "my_root/some_child");

    assert(!ct_path_is_under_path(root, root));
    assert(ct_path_is_under_path(child, root));
    assert(!ct_path_is_under_path(root, child));
    assert(!ct_path_is_under_path(other, root));
    assert(ct_path_is_under_path(relative_child, relative_root));

    ct_path_free(relative_child);
    ct_path_free(relative_root);
    ct_path_free(other);
    ct_path_free(child);
    ct_path_free(root);
}


int
main(int argc, char *argv[])
{
    test_path_alloc();
    test_path_alloc_copy();
    test_path_alloc_with_abs_given_path();
    test_path_alloc_with_relative_given_path();
    test_path_alloc_with_root_working_dir();
    test_path_equals_path();
    test_path_is_under_path();
    return EXIT_SUCCESS;
}

