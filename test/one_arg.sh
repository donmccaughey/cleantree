#!/bin/sh

status=0
mkdir test_root

if ./cleantree test_root
then
    status=1
fi

rmdir test_root
exit $status

