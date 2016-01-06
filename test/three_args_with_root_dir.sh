#!/bin/sh

status=1
mkdir test_root

if ./cleantree test_root test_root/file1 test_root
then
    status=0
fi

rmdir test_root
exit $status

