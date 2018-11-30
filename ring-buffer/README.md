Project 6: Ring Buffer
Author: Noah Rudin
CS253 Section 2
Fall 2017

-OVERVIEW-

This project requires the student to make a ring buffer, which handles size 
issues when writing to an array while reading from memory. This programming 
assignment requires the programmer to refine skills such as signals, pointers, 
file operations, and general coding practices.

-COMPILING AND USING- *STRAIGHT FROM ASSIGNMENT DESCRIPTION*

To run the test programs you will need to set the paths to find the library:

```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib
```

Then run the test programs as follows:

```
./grader
```

OR, the run.sh script will set the path for you and show proper usage

```
./run.sh
```

To run the autograder before submitting, run:

```
./backpack.sh
```


-TESTING-

The majority of testing was done via the grading scripts provided within 
C-examples. I used these scripts to tweak different issues I had concerning the 
ring buffer. One issue that I had the most trouble with was the alarm signal, 
and setting the flag init to true and false. I remedied this by doing a check 
within my dump_buffer method to set init to false if it wasn't.

-SOURCES USED-

-Worked with Farhad and classmates (Spencer Saunders and Davis Plumlee)

-C-examples directory in CS253 resources

-Lemoda for setting and getting environment vars
https://www.lemoda.net/c/set-get-env/

-KNOWN ISSUES AND BUGS-

There are no known errors in this code.
