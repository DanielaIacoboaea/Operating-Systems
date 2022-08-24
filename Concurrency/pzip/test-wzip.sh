#! /bin/bash

if ! [[ -x wzip ]]; then
    echo "wzip executable does not exist"
    exit 1
fi

../../../course/ostep-projects-master/tester/run-tests.sh $*


