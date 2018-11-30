#!/bin/bash
#Setup our library path so the grader will run 
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib

#make project
make

clear

#run grader
./grader
