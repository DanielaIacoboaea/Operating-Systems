#! /bin/bash

if ! [[ -x wgrep ]]; then
    echo "wgrep executable does not exist"
    exit 1
fi

../../course/ostep-projects-master/tester/run-tests.sh $*



