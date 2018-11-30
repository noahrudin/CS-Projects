#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

// P2: Word Count
// Author: Noah Rudin
// CS253-002 Fall 2017

// This program parses a given text file to return the counts of chars, 
// lines, words, digits, and a specified match string.
 
int main(int argc, char *argv[])
{

//All integer declarations
int i=0;
int x=0;
int c;
int state=0;
int numWords=0;
int numChars=0;
int numLines=0;
int numZeros=0;
int numOnes=0;
int numTwos=0;
int numThrees=0;
int numFours=0;
int numFives=0;
int numSixes=0;
int numSevens=0;
int numEights=0;
int numNines=0;
int numMatches=0;
int mFlag=0;
int hFlag=0;
	
//Checks for 0 args
if(argc<2||strlen(argv[1])<2){
	printf("Arg counts of less than 2 must only contain '-h'.\nSample input: ./wc-match -m test < input\n");
	exit(0);
	}
//Parses the args on the command line
 while ((x = getopt (argc, argv, "hm:")) != -1){
    switch (x)
      {
      case 'h':
	//sets a flag for only printing help
        hFlag++;
        break;
      case 'm':
	//sets a flag for executing the word match algorithm
        mFlag++;
        break;
      case '?':
        if (optopt == 'm'){
	//handles all cases for which there is '-m' but incorrect following args
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
       } else if (isprint (optopt)){
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
       } else{
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
}
      default:
        abort();
      }
}

//beginning of word match algorithm
if(mFlag==1){
  int flag=0;
  int flag1=0;
  int word[strlen(argv[2])];
//fills word match array with the input string
  for (i=0;i<strlen(argv[2]);++i){
    word[i]=argv[2][i];
  }
//reads in file
  while((c=getchar())!=EOF){
//incrementing numWords via a state machine
    if (c == ' ' || c == '\n' || c == '\t') {
      state = 0;
    } else if (state == 0) {
	  state = 1;
          ++numWords;
    }
//checks for new lines
  if(c=='\n'){
    ++numLines;
  }
//Checks for all chars
  if(c != ' ' || c != '\n' || c != '\t'){
    ++numChars;
  }
//Switch statements for allocating and incrementing digit values
  if(isdigit(c)){
    switch(c){
      case 48:
        ++numZeros;
      break;
      case 49:
        ++numOnes;
      break;

      case 50:
        ++numTwos;
      break;
      case 51:
        ++numThrees;
      break;
      case 52:
        ++numFours;
      break;
      case 53:
        ++numFives;
      break;
      case 54:
        ++numSixes;
      break;
      case 55:
        ++numSevens;
      break;
      case 56:
        ++numEights;
      break;
      case 57:
        ++numNines;
      break;
    }

  }
//Word matching state machine, utilizes two flags to increment numMatches
  if(c==word[flag]&&flag>=1){
    flag++;
    if(flag==strlen(argv[2])){
      numMatches++;
    }
  }else{
   flag=0;
  }
  if(c==word[0]&&flag==0){
    flag=1;
    flag1=flag1+1;
   }else{
    flag1=0;
    }
   }
}
//prints usage for arg '-h'
if(hFlag==1){
  printf("Usage:\nFor word match, input -m <match> < fileIN > fileOUT\nFor help, input -h\n");
  return(0);
}
//final results and output
  printf("-Word Matcher Complete-\n\n");
  printf("words: %d\n", numWords);
  printf("chars: %d\n", numChars);
  printf("lines: %d\n", numLines);
  printf("digit 0: %d\n", numZeros);
  printf("digit 1: %d\n", numOnes);
  printf("digit 2: %d\n", numTwos);
  printf("digit 3: %d\n", numThrees);
  printf("digit 4: %d\n", numFours);
  printf("digit 5: %d\n", numFives);
  printf("digit 6: %d\n", numSixes);
  printf("digit 7: %d\n", numSevens);
  printf("digit 8: %d\n", numEights);
  printf("digit 9: %d\n", numNines);
  printf("matched %s: %d\n", argv[2], numMatches);

return (0);
}
