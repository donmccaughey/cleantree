#!/bin/sh

status=1
mkdir test_root
touch test_root/file1
touch test_root/file2
touch test_root/file3
touch test_root/file4

if ./cleantree test_root test_root/file1 test_root/file2
then
    status=0
fi

if test ! -f test_root/file1 -o ! -f test_root/file2
then
    status=1
fi

if test -f test_root/file3 -o -f test_root/file4
then
    status=1
fi

rm -rf test_root
exit $status

