#!/bin/sh

status=1
mkdir test_root

if ../src/cleantree test_root file1
then
    status=0
fi

rmdir test_root
exit $status

