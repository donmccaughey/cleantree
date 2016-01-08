#!/bin/sh

status=1
mkdir test_root
mkdir test_root/dir1
mkdir test_root/dir2
mkdir test_root/dir3
mkdir test_root/dir4

if ./cleantree test_root test_root/dir1 test_root/dir2
then
    status=0
fi

if test ! -d test_root/dir1 -o ! -d test_root/dir2
then
    status=1
fi

if test -d test_root/dir3 -o -d test_root/dir4
then
    status=1
fi

rm -rf test_root
exit $status

