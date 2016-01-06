#ifndef CLEANTREE_PATH_SET_H_INCLUDED
#define CLEANTREE_PATH_SET_H_INCLUDED


struct ct_path;


enum ct_path_set_error {
    ct_path_set_error_none = 0,
    ct_path_set_error_errno,
    ct_path_set_error_not_under_root_dir,
    ct_path_set_error_path_equals_root_dir,
    ct_path_set_error_duplicate_path,
};


struct ct_path_set {
    int count;
    struct ct_path **paths;
    struct ct_path *root_dir;
};


struct ct_path_set *
ct_path_set_alloc(struct ct_path const *root_dir);

void
ct_path_set_free(struct ct_path_set *path_set);

int
ct_path_set_add_path(struct ct_path_set *path_set,
                     struct ct_path const *path,
                     enum ct_path_set_error *error);


#endif

