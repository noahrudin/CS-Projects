# NFA to DFA

* Author: Noah Rudin, Bryant Barreto
* Class: CS361 Section 2
* Semester: Spring 2018

---

## Overview

This Java application creates an NFA given a set of states and transitions and 
an alphabet, and returns a DFA.

---

## Compiling and Using

To compile, execute this command:
```
$ javac fa/nfa/NFADriver.java
```

Run the class with the following command:
```
$ java fa.nfa.NFADriver .//tests/p2tc0.txt
```

This program will then read the sample test file to generate an NFA and DFA.

---

## Discussion

The hardest part of this project by far was the getDFA() method we had to 
complete. We repeatedly received multiple null pointer exceptions in our DFA, 
and the program was not outputting the correct states or the correct number of 
states. The entire program was difficult in itself to wrap our heads around 
conceptually as well, since we needed a mind-boggling number of separate 
LinkedHashSets to not only store states, but states in relation to one another 
and then transitions between them.

We started to address the null pointer issue with running System.out.println() 
calls that output the set of states at different points within our getDFA() 
method loops. We found that a simple error was making several delta transitions 
become null. We had a temporary state variable that we cleared every time the 
loop was called, which we thought was correct. However, the clear() method that 
we called actually just replaced the state name with an empty string, not 
starting over with a fresh state. We decided to re-instantiate it each time in 
the loop to remedy this issue and it appeared to work.

Another small issue that we had was the output not correctly showing the start 
state (but correctly showing all the other states). This simply required some 
hard looks at the code to decipher what the issue was. Bryant and I had 
accidentally mixed up the order in which we were queuing and de-queuing our 
head 
node, and then the error was resolved.

---

## Extra Credit

None.

## Sources used

Sources used include:

CS361 Piazza Q&A Page: https://piazza.com/class/jbnuftnjzu67jl
Java LinkedHashSet API: 
https://docs.oracle.com/javase/7/docs/api/java/util/LinkedHashSet.html
Classmates' ideas and techniques
