#!/bin/sh

status=0

if ../src/cleantree missing_root file1
then
    status=1
fi

exit $status

