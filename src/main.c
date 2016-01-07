#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

#include "options.h"
#include "path.h"
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

    printf("cleaning tree at %s\n", options->root_dir->abs_path);
    printf("%i paths to keep\n", options->paths_to_keep->count);
    for (int i = 0; i < options->paths_to_keep->count; ++i) {
        printf("    %s\n", options->paths_to_keep->paths[i]->abs_path);
    }

    struct ct_path_set *paths_found = ct_path_set_alloc(options->root_dir);
    if (!paths_found) {
        perror(options->command_name);
        return EXIT_FAILURE;
    }

    int result = ct_path_set_find_all(paths_found);
    if (-1 == result) {
        perror(options->command_name);
        return EXIT_FAILURE;
    }

    printf("found %i paths\n", paths_found->count);
    for (int i = 0; i < paths_found->count; ++i) {
        printf("    %s\n", paths_found->paths[i]->abs_path);
    }

    ct_path_set_free(paths_found);
    ct_options_free(options);
    return EXIT_SUCCESS;
}

