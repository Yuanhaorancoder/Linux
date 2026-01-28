#!/bin/bash

cnt=10
while [ $cnt -gt 0 ]
do
    echo "hello shell: ${cnt}"
    let cnt--
  done
