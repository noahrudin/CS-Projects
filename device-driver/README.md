# Project 5: Device Driver

* Author: Noah Rudin
* Class: CS453 [Operating Systems] Section 2


## Overview

The mydash program is a shell that accepts arguments next to commands and 
other, more basic commands such as cd, ls, pwd. and exit. In addition, the user 
may append an ampersand after any input to send the desired process into the 
background, where the shell will report its completion after hitting enter. The 
user may check on the status of background processes with the command "jobs".

## Manifest

### Makefile - supplies rules to compile the C code into executable
### booga.c - main program code that contains most device driver functionality
### booga.h - header file for declaring rules and functions for booga.c
### README.md - this file, provides a user with information regarding booga
### test-booga.c - driver class for booga.c
### TestCases.sh - tests written to help test booga.c, containg commands with fringe cases and odd processes

## Building the project

To build this code, code by typing "make" within the booga source folder.

## Features and usage

To use, first mount all necessary devices by typing ./booga_load with sudo permission.
After the drivers are mounted, the user can read and write bytes with the following usage:
"./test-booga <device number> <buffersize> <r|w>"

This will either select and print a random string via the specified device, or "write" bits to that device.

Additionally, running "cat /proc/driver/booga" will show the stats of the session.

To safely stop using booga, type ./booga_unload with sudo permission.

## Testing

To test my project, I used booga-test1.sh and booga-test2.sh to set a baseline after writing the code.
I realized that these two tests, while good at making sure the program did not crash for
basic functionality, was limiting in the number of things that would be tested. I built off of this by making
TestCases, a script that is much more intensive in its testing in addition to the initial tests.


### Known Bugs

No known bugs

## Reflection and Self Assessment

As far as writing code went, the toughest part of the project was implementing the read method.
More specifically, the process of copying to the buffer was giving me trouble and causing constant kernel crashing.
I realized that this was an issue with how I was remembering the concept of a char*. I had
forgotten that a char* variable is the equivalent to a static array of chars, which is malloc'd
to a certain size and causes a crash when it is populated beyond its capacity. The memory leaks over
and crashes the kernel. After I had figured out how to populate the intermediate string, copying 
it to the user space was much easier since I had an example to reference. The write method was not
too bad at all, since it was a mock write. I only had to be careful how I was incrementing the bytes
written. The stats section gave me a small amount of trouble with finding out which device was being
opened. I realized that NUM(inode->i_rdev) returned the int corresponding to the device name (0->booga0),
etc. Once that was tracked, the other outputs were pretty easy to find and format. I think I did a good job
pacing myself on this project, and even though I got hung up on the booga_read() method I was able to 
implement the other tasks and then come back and complete it once my understanding had increased.


## Sources used

[Signals and Sigterm](http://cs-pub.bu.edu/fac/richwest/cs591_w1/notes/wk3_pt2.PDF)

[Copy to User Manual](https://www.fsl.cs.sunysb.edu/kernel-api/re256.html)

[Path of a file](https://stackoverflow.com/questions/229012/getting-absolute-path-of-a-file)

[Kernel Panic](https://en.wikipedia.org/wiki/Kernel_panic)

[Kernel Dump Troubleshooting](https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/kernel_administration_guide/kernel_crash_dump_guide)

Example code found in examples/device-management/linux-device-drivers/example4c


