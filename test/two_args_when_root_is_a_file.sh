#!/bin/sh

status=0
touch root_file

if ./cleantree root_file root_file/file1
then
    status=1
fi

rm root_file
exit $status

