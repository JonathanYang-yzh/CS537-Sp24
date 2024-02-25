#! /bin/csh -f
set TEST_HOME = /p/course/cs537-oliphant/tests/P0
set source_file = hw.c
set binary_file = a.out
set bin_dir = ${TEST_HOME}/bin
set test_dir = ${TEST_HOME}/tests

${bin_dir}/generic-tester.py -s $source_file -b $binary_file -t $test_dir $argv[*]
