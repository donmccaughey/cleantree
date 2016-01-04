#ifndef CLEANTREE_PATH_SET_H_INCLUDED
#define CLEANTREE_PATH_SET_H_INCLUDED


struct ct_path;


struct ct_path_set {
    int count;
    struct ct_path *root_dir;
    struct ct_path **paths;
};


struct ct_path_set *
ct_path_set_alloc(struct ct_path const *root_dir);

void
ct_path_set_free(struct ct_path_set *path_set);


#endif

