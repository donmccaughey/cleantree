EXTRA_DIST = LICENSE README.md

bin_PROGRAMS = cleantree
cleantree_SOURCES = src/main.c \
                    src/options.h src/options.c \
                    src/path.h src/path.c

check_SCRIPTS = test/no_args.sh \
                test/one_arg.sh \
                test/two_args.sh \
                test/two_args_with_missing_root.sh \
                test/two_args_when_root_is_a_file.sh
EXTRA_DIST += $(check_SCRIPTS)
TESTS = $(check_SCRIPTS)

