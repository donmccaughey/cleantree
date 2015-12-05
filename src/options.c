#include "options.h"
#include <config.h>

#include <getopt.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static struct option long_options[] = {
    {
        .name="help",
        .has_arg=no_argument,
        .flag=NULL,
        .val='h'
    },
    {
        .name="version",
        .has_arg=no_argument,
        .flag=NULL,
        .val='V'
    },
    {
        .name=NULL,
        .has_arg=no_argument,
        .flag=NULL,
        .val=0
    }
};

static char const short_options[] = "hV";


static int
get_options(struct ct_options *options, int argc, char *argv[]);

static void
print_help(struct ct_options *options);

static void
print_version(struct ct_options *options);


struct ct_options *
ct_options_alloc(int argc, char *argv[])
{
    struct ct_options *options = calloc(0, sizeof(struct ct_options));
    if (!options) return NULL;

    char *command_name = basename(argv[0]);
    if (!command_name) {
        ct_options_free(options);
        return NULL;
    }

    options->command_name = strdup(command_name);
    if (!options->command_name) {
        ct_options_free(options);
        return NULL;
    }
    
    int result = get_options(options, argc, argv);
    if (-1 == result) {
        ct_options_free(options);
        return NULL;
    }

    return options;
}


int
ct_options_exit_status(struct ct_options const *options)
{
    return options->error ? EXIT_FAILURE : EXIT_SUCCESS;
}


void
ct_options_free(struct ct_options *options)
{
    if (options) {
        free(options->command_name);
        free(options);
    }
}


bool
ct_options_should_exit(struct ct_options const *options)
{
    return options->error || options->help || options->version;
}


static int
get_options(struct ct_options *options, int argc, char *argv[])
{
    int ch;
    int long_option_index;
    while (-1 != (ch = getopt_long(argc, argv, short_options, long_options, &long_option_index))) {
        switch (ch) {
            case 'h':
                options->help = true;
                break;
            case 'V':
                options->version = true;
                break;
            default:
                options->error = true;
                break;
        }
    }

    if (options->error || options->help)  {
        print_help(options);
        return 0;
    }

    if (options->version) {
        print_version(options);
        return 0;
    }

    int remaining_arg_count = argc - optind;
    
    if (0 == remaining_arg_count) {
        options->error = true;
        fprintf(stderr, "%s: expected ROOT_DIR argument\n", options->command_name);
        print_help(options);
        return 0;
    }
    return 0;
}


static void
print_help(struct ct_options *options)
{
    FILE *out = stdout;
    if (options->error) out = stderr;

    fprintf(out, "Usage: %s [OPTIONS]... ROOT_DIR [FILES_TO_KEEP]...\n", options->command_name);
    fprintf(out, "Remove unexpected files from a directory tree.\n");
    fprintf(out, "\n");
    fprintf(out, "  -h, --help          display this help message and exit\n");
    fprintf(out, "  -V, --version       display the version number and exit\n");
    fprintf(out, "\n");
    fprintf(out, "Report bugs to <%s>\n", PACKAGE_BUGREPORT);
}


static void
print_version(struct ct_options *options)
{
    if (0 == strcasecmp(options->command_name, PACKAGE_NAME)) {
        printf("%s\n", PACKAGE_STRING);
    } else {
        printf("%s (%s) %s\n", options->command_name, PACKAGE_NAME, PACKAGE_VERSION);
    }
}

