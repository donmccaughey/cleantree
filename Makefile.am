EXTRA_DIST = LICENSE README.md

bin_PROGRAMS = cleantree
cleantree_SOURCES = src/main.c \
                    src/options.h src/options.c \
                    src/path.h src/path.c \
                    src/path_set.h src/path_set.c

check_PROGRAMS = test/path_test
test_path_test_SOURCES = test/path_test.c \
                               src/path.h src/path.c
check_SCRIPTS = test/no_args.sh \
                test/one_arg.sh \
                test/two_args.sh \
                test/two_args_with_missing_root.sh \
                test/two_args_when_root_is_a_file.sh \
                test/three_args.sh \
                test/three_args_with_one_not_under_root.sh
EXTRA_DIST += $(check_SCRIPTS)
TESTS = $(check_PROGRAMS) $(check_SCRIPTS)

