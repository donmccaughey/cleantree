#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

#include "options.h"
#include "path_set.h"


int
main(int argc, char *argv[])
{
    struct ct_options *options = ct_options_alloc(argc, argv);
    if (!options) {
        perror(basename(argv[0]));
        return EXIT_FAILURE;
    }

    if (ct_options_should_exit(options)) {
        return ct_options_exit_status(options);
    }

    struct ct_path_set *paths_found = ct_path_set_alloc(options->root_dir);
    if (!paths_found) {
        perror(options->command_name);
        return EXIT_FAILURE;
    }

    ct_path_set_free(paths_found);
    ct_options_free(options);
    return EXIT_SUCCESS;
}

