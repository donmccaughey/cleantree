#!/bin/sh

status=0

if ./cleantree missing_root missing_root/file1
then
    status=1
fi

exit $status

