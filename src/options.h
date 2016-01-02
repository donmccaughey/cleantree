#ifndef CLEANTREE_OPTIONS_H_INCLUDED
#define CLEANTREE_OPTIONS_H_INCLUDED


#include <stdbool.h>


struct ct_path;


struct ct_options {
    char *command_name;
    bool error;
    bool help;
    struct ct_path *root_dir;
    bool version;
};


struct ct_options *
ct_options_alloc(int argc, char *argv[]);

void
ct_options_free(struct ct_options *options);

int
ct_options_exit_status(struct ct_options const *options);

bool
ct_options_should_exit(struct ct_options const *options);


#endif

