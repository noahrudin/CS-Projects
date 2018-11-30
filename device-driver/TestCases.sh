#!/bin/bash

sudo ./booga_load
sudo ./booga_unload


sudo ./booga_load

./test-booga 0 0 r
./test-booga 0 1 r
./test-booga 0 2 r
./test-booga 0 10 r

./test-booga 0 0 r | wc -m
./test-booga 0 1 r | wc -m
./test-booga 0 2 r | wc -m

./test-booga asd fgh r
./test-booga asd dfg w
./test-booga asd dfg q
./test-booga /dev/ t r
./test-booga /dev/ t r
./test-booga 3.0 1.0 r

./test-booga 3 1.0 r
./test-booga 3 1000 r
./test-booga 3 1000 w
./test-booga 3 1000 w
./test-booga 3 1000 w

cat /proc/driver/booga


sudo ./booga_unload