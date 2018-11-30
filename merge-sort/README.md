# Project: Merge Sort
### Author: Noah Rudin, Shane Panter
### CS453 Fall 2018


# Introduction

This folder contains a serial implementation of mergesort.

# Manifest

Makefile         Build file.

multi-threaded/mergesort.c      The source file that contains the mergesort and related functions.

multi-threaded/mergesortTest.c  The driver that generates random data, runs and times mergesort.

multi-threaded/timing.c         The timing functions.

multi-threaded/test1.sh         Try different values of insertion sort threshold to find best value of threshold to 			 switch to insertion sort.

multi-threaded/test2.sh         Time mergesort for different values of n.


# Building and Running

To build, use:

`make`

In either the single-threaded folder, run as follows:


`mergesort <input size> [<seed>]`

In either the single-threaded folder, run as follows:


`mergesort <input size> <number of threads> [<seed>]`

# Observations

Current Testing Machine:
* OS: Kali Linux GNOME v 3.28.2
* CPU: Intel i5-6300U Quad Core (4 Cores)
* GPU: Intel HD Graphics (Skylake GT2)

### Testing 100,000,000 Elements with and Without Seed, Ranging between 1 thread and 6 threads

* All Testing was done within a 10-minute time window, with the same applications running in background

|           | No Seed | 10     | 20     | 40     |
|-----------|---------|--------|--------|--------|
| 1 Thread  | 12.80s  | 12.62s | 12.59s | 12.57s |
| 2 Threads | 6.73s   | 7.13s  | 6.74s  | 6.70s  |
| 3 Threads | 5.90s   | 5.98s  | 5.89s  | 5.93s  |
| 4 Threads | 5.27s   | 5.24s  | 5.26s  | 5.25s  |
| 5 Threads | 5.30s   | 5.37s  | 5.34s  | 5.28s  |
| 6 Threads | 5.42s   | 5.44s  | 5.43s  | 5.36s  |
| 7 Threads | 5.49s   | 5.46s  | 5.51s  | 6.10s  |
| 8 Threads | 5.69s   | 6.07s  | 5.78s  | 6.02s  |

* For each section of testing, the time follows a parabola that shows the fastest time at 4 cores.
* This implies the conclusion that the fastest performance comes from using the full number of cores available.
