#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

#include "options.h"


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

    printf("Hello, world!\n");

    ct_options_free(options);
    return EXIT_SUCCESS;
}

