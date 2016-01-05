#!/bin/sh

status=0
mkdir test_root

if ./cleantree test_root test_root/file1 not_root/file2
then
    status=1
fi

rmdir test_root
exit $status

