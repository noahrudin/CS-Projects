# Project 3: MyDash Part 2

* Author: Noah Rudin
* Class: CS453 [Operating Systems] Section 2

---

## Overview

The mydash program is a shell that accepts arguments next to commands and 
other, more basic commands such as cd, ls, pwd. and exit. In addition, the user 
may append an ampersand after any input to send the desired process into the 
background, where the shell will report its completion after hitting enter. The 
user may check on the status of background processes with the command `jobs`.

---

## Manifest

### Makefile - supplies rules to compile the C code into executable
### mydash-src/mydash.c - main program code that contains most mydash functionality
### mydash/mydash.h - header file for declaring rules and functions for mydash.c
### README.md - this file, provides a user with information regarding mydash
### valgrind.supp - a list of rules to suppress on Valgrind
### TestCases - tests written to help test mydash, containg commands with fringe cases and odd processes

---

## Building the project

To build this code, first install readline from:

https://tiswww.case.edu/php/chet/readline/rltop.html

After this, set the library path so that gcc can find the shared library folder. Type this:

`export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:lib`

Next, compile the source code by typing `make` within the mydash folder.

---

## Features and usage

To use mydash, type ./mydash to launch the executable. While the program runs, 
it should function similar to a command-line terminal. Mydash contains basic 
command functionality such as cd, ls, pwd, and exit. Additionally, the user may 
type an ampersand after a command (e.g.- `ls&`) to start the argument process 
in the background. Typing in `jobs` will output the status of all background 
processes. Each completed job will be reported only once.

To use mydash with valgrind, type valgrind --leak-check=full ./mydash to launch 
mydash with valgrind
feedback.

---

## Testing

To test the mydash program, I leaned on the supplied unit tests, as well as 
generating my own that input several odd arguments to try to 
break the program, such as putting unique processes in the background. These 
tests allowed me to realize different areas where common commands would work, 
but odd cases that should still technically work would break. For example, 
typing in `sleep 5&` twice and then hitting jobs originally would output the 
process four times instead of two. This is detailed in the bugs section.

---

### Valgrind

After extensive testing and patching with Valgrind, the leak situation is much 
improved, but the bill of health is still not yet perfect. The exit command 
does not perfectly free all lists or jobs, but I am happy that most, if not all, 
of the other possible commands do not report any leaks. Exit is the only known 
problematic command.

---

### Known Bugs

Slight valgrind error on exit, as well as printing each background job twice on `jobs` command. Recreate with the below commands:

- `sleep 10&`
- `sleep 10&`
- `jobs`

This will show each command twice.

---

## Reflection and Self Assessment

I spent the biggest amount of time during this project working on the printList 
method that was supplied with the sample code. I was frustrated initially about 
how I could not see how the printList function worked, but I ended up creating 
my own printAllJobs method that traversed the jobs linked list and output the 
status of each unreported/running job. In addition to this, I felt that 
handling signals required an unexpected number of hours to complete. 
Specfically, I needed to teach myself the distinctions between several 
different types of signals and signal codes. I believe the current version of 
mydash contains the correct signal handling methods and flags, but it is 
definitely a topic I would like to learn more about within the field of C 
programming.

While that part of the project took the longest time-wise, the most trying part 
of the project patience-wise was resolving the many valgrind leaks within 
mydash. I improved a few things from mydash part 1, in which I failed to trim 
the null-terminating character at the end of a string. At the end of the day, I 
would have liked to go back and resolve all of my errors within Valgrind to 
create safe code.

---

## Sources used

[Learning About LongJmp](https://stackoverflow.com/questions/16828378/readline-get-a-new-prompt-on-sigint)

[Link Help](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet#links)


