#ifndef	__MYDASH_H
#define	__MYDASH_H

#include	<sys/types.h>
#include	<stdio.h>
#include	<stdlib.h>	
#include 	<setjmp.h>	
#include	<sysexits.h>
#include	<string.h>
#include	<unistd.h>
#include	<pwd.h>

#include    "../include/Node.h"
#include    "../include/List.h"	

#define MAX_LENGTH 1024	
#define MAX_TOKENS 2048
#define	MAXLINE	4096
#define MAX_ARGS 2048

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default file access permissions for new files */
#define	DIR_MODE	(FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)
					/* default permissions for new directories */


/*
 * job struct, holds info and flags for background execution
 */
struct job {
	int id;
	int* status;
	char* isRunning;
	pid_t pid;
	pid_t pgid;
	int bg;
	char* cmd;
};

/*
 * Runs a built-in shell command
 */ 
void runBuiltIns(char** argv, struct list* jobs, struct passwd* pwuid);

/*
 * Allows execution of external commands  
 */ 
void run(char** argv, char* input, struct list* jobs, int* background, int* nextID, pid_t pgID);

/*
 * Functions to print running and done background jobs
 */
void printAllJobs(int flag, struct list* jobs);
void printJobs(int id, int pid, char* running, char* cmd);

/*
 * cd command which changes to the desired directory. If no argument
 * or ~, the home directory is selected
 */ 
void changeDir(struct passwd* pwuid, char** argv, char* dir);

/*
 * Outputs history
 */
void printHistory(void);

/*
 * Checks for empty cmd
 */
int isEmptyCommand(char* input);

/*
 * Removes excess whitespace from input
 */ 
void removeWhiteSpace(char* str);

/*
 * Parses user input, removes "&" and returns a tripped flag if "&" is present
 */ 
int parseAmpersand(char* in);

/*
 * Populates the args array with user input, utilizes parseAmpersand
 */ 
void parse(char* input, int* background, char* args[]);

/*
 * If a -v flag exists, output current git version
 */ 
void version(int argc, char** argv);

/*
 * Attached list tests
 */
int equals(const void *obj, const void *other);
char *toString(const void *obj);
void freeObject(void *obj);

/*
 * Creates myDash prompt
 */
char* generatePrompt(void);

/*
 * Signal handler function, generates fresh prompt on SIGINT
 */ 
void signalHandler(int signo);


/*
 * Method that saves set point for later jump
 */
int sigsetjmp(sigjmp_buf env, int savemask);


void	err_dump(const char *, ...);
void	err_msg(const char *, ...);
void	err_quit(const char *, ...);
void	err_ret(const char *, ...);
void	err_sys(const char *, ...);



char **parseInput(char *s, char *delimiter, int *numTokens);

#endif	/* __MYDASH_H */
