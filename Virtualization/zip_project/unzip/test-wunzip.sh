#! /bin/bash

if ! [[ -x wunzip ]]; then
    echo "wunzip executable does not exist"
    exit 1
fi

../../../course/ostep-projects-master/tester/run-tests.sh $*


