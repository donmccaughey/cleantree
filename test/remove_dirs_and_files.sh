#!/bin/sh

status=1
mkdir test_root
mkdir test_root/dir1
touch test_root/dir1/file1
mkdir test_root/dir2
mkdir test_root/dir2/subdir1
touch test_root/dir2/subdir1/file2
touch test_root/dir2/subdir1/file3
mkdir test_root/dir2/subdir2
touch test_root/dir2/subdir2/file4
mkdir test_root/dir3

if ./cleantree test_root \
        test_root/dir1/file1 \
        test_root/dir2/subdir1/file2 \
        test_root/dir2/subdir2
then
    status=0
fi

if test \
           ! -f test_root/dir1/file1 \
        -o ! -f test_root/dir2/subdir1/file2 \
        -o ! -d test_root/dir2/subdir2 
then
    status=1
fi

if test \
           -f test_root/dir2/subdir1/file3 \
        -o -f test_root/dir2/subdir2/file4 \
        -o -d test_root/dir3
then
    status=1
fi

rm -rf test_root
exit $status

