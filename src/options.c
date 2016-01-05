#include "options.h"
#include <config.h>

#include <getopt.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "path.h"
#include "path_set.h"


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
    struct ct_options *options = calloc(1, sizeof(struct ct_options));
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
        ct_path_set_free(options->paths_to_keep);
        ct_path_free(options->root_dir);
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
    
    if (remaining_arg_count < 1) {
        options->error = true;
        fprintf(stderr, "%s: expected ROOT_DIR argument\n", options->command_name);
    }

    if (remaining_arg_count < 2) {
        options->error = true;
        fprintf(stderr, "%s: expected one or more PATH_TO_KEEP arguments\n", options->command_name);
        print_help(options);
        return 0;
    }
    
    options->root_dir = ct_path_alloc(NULL, argv[optind]);
    if (!options->root_dir) return -1; 
    if (!options->root_dir->exists) {
        options->error = true;
        fprintf(stderr, "%s: ROOT_DIR \"%s\" does not exist\n",
                options->command_name, options->root_dir->given_path);
        print_help(options);
        return 0;
    }
    if (!options->root_dir->is_dir) {
        options->error = true;
        fprintf(stderr, "%s: ROOT_DIR \"%s\" is not a directory\n",
                options->command_name, options->root_dir->given_path);
        print_help(options);
        return 0;
    }

    options->paths_to_keep = ct_path_set_alloc(options->root_dir);
    if (!options->paths_to_keep) return -1;

    for (int i = optind + 1; i < argc; ++i) {
        struct ct_path *path_to_keep = ct_path_alloc(NULL, argv[i]);
        if (!path_to_keep) return -1;
        
        enum ct_path_set_error error;
        int result = ct_path_set_add_path(options->paths_to_keep, path_to_keep, &error);
        if (-1 == result) {
            if (ct_path_set_error_errno == error) return -1;
            if (ct_path_set_error_not_under_root_dir == error) {
                options->error = true;
                fprintf(stderr, "%s: \"%s\" is not under ROOT_DIR \"%s\"\n",
                        options->command_name, path_to_keep->given_path,
                        options->root_dir->given_path);
            }
        }
        ct_path_free(path_to_keep);
    }
    if (options->error) {
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

    fprintf(out, "Usage: %s [OPTIONS]... ROOT_DIR PATH_TO_KEEP...\n", options->command_name);
    fprintf(out, "Remove unexpected files and directories from a directory tree.\n");
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

