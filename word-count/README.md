# Project 2: Word Count
### Author: Noah Rudin
### Class: CS253 Section 2

---

# OVERVIEW:
This program is intended to take in a text file, parse the file, and count the number of chars, words, lines, and instances of digits 0-9. The number of matches in a file to a user-designated word (given in the command line) is also displayed.

---

# COMPILING AND USING:
- To compile and run the word match code, after creating the executable, type "./wc-match -m 'word query' < 'input file'", in which word query is the desired word to be matched and input file is the text to parse.

- To output the results to be printed in the console to a text file, type `./wc-match -m 'word query' < 'input file' > 'output file'`, in which output file is the name of the file in which the console display is written. "input" has been provided for you which reads "Hello there, I am a sample file."

- To simply output the usage menu, type `./wc-match -h`.

---

# TESTING:
Originally I had a hard time with creating a system of flags to mark that a match had been created, which is what I spent the most time pondering on. I extensively tested the program once the word count was implemented, matching the results from data, data1 and data2 with the autograder executable. I also created my own text files and parsed those, and compared all results with the grader. For example, an organic file I wrote contained 5 lines of "hellohellhello", and the autograder and my program displayed the same answer and statistics on the console.

---

# SOURCES USED:

- ASCII Values for chars-
http://www.asciitable.com/

- https://www.gidforums.com/t-7970.html

- Help with STDIN- 
https://stackoverflow.com/questions/16869467/command-line-arguments-reading-a-file

- Various help with data in- 
https://stackoverflow.com/questions/16400886/reading-from-a-file-word-by-word

---

# KNOWN ISSUES:
There are no known issues at this time. The program seems to correctly output all text files and test suites.
