#! /bin/bash

if ! [[ -x wcat ]]; then
    echo "wcat executable does not exist"
    exit 1
fi

../course/ostep-projects-master/tester/run-tests.sh $*


