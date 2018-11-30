#include    <sys/types.h>
#include    <sys/wait.h>
#include    <mydash.h>
#include 	<setjmp.h>
#include    <readline/readline.h>
#include    <readline/history.h>
#include    <signal.h>
#include    <stdio.h>
#include    <pwd.h>
#include    <stdlib.h>
#include    <string.h>
#include    <unistd.h>
#include    "../include/Node.h"
#include    "../include/List.h"


struct list* jobs;
sigjmp_buf SIGINT_BUF;

int main(int argc, char** argv)
{
	char* prompt = generatePrompt();

	uid_t uid = getuid();
	pid_t pgID; 
	struct passwd* pwuid = getpwuid(uid);
	int nextID = 1;	
	char* buffer = (char*)malloc(MAXLINE);
	char* jobName;
	int background;
	
	version(argc, argv);
	using_history();
	//init jobs
    jobs = createList(equals, toString, freeObject);

	if((setpgid(0, 0)) < 0)
		err_sys("Could not set pgid");

	//set group ID	
	pgID = getpgrp();	
	
	while ( sigsetjmp( SIGINT_BUF, 1 ) != 0 );

	//General shell signal ignores, SIGINT trips new line
	signal(SIGTTOU, SIG_IGN);
	signal(SIGINT, signalHandler);
	signal(SIGTSTP, SIG_IGN);

	while((buffer = readline(prompt)) != NULL) {
		if(!isEmptyCommand(buffer)) {
			
			jobName = (char*)malloc(MAXLINE);
			strcpy(jobName, buffer); 
			add_history(buffer);
			parse(buffer, &background, argv);

			//check for built in commands
			if((strcmp(buffer, "exit") == 0) || (strcmp(buffer, "Exit") == 0) || (strcmp(buffer, "cd") == 0) || (strcmp(buffer, "history") == 0) || (strcmp(buffer, "jobs") == 0)) {
				runBuiltIns(argv, jobs, pwuid);
			}
			else{
				run(argv, jobName, jobs, &background, &nextID, pgID);
				nextID++;
			}
		}else{
			int i = getSize(jobs);
			while(i > 0){
				printAllJobs(1, jobs);
				i--;
			}
		}
		signal(SIGTTOU, SIG_IGN);
		free(buffer);
	}
	printf("\n");
	return 0;

}

void runBuiltIns(char** argv, struct list* jobs, struct passwd* pwuid)
{
	char dir[1024];
	if(strcmp(argv[0], "cd") == 0) {
		changeDir(pwuid, argv, dir);
	}
	else if(strcmp(argv[0], "history") == 0) {
		printHistory();
	}
	else if(strcmp(argv[0], "exit") == 0 || strcmp(argv[0], "Exit") == 0) { 
        	while(!isEmpty(jobs)){
            	removeRear(jobs);
        	}
        	freeList(jobs);
        	exit(0);
		}else if(strcmp(argv[0], "jobs") == 0) {
			int i = getSize(jobs);
			while(i > 0){
				printAllJobs(1, jobs);
				i--;
			}	
	}
}


void run(char** argv, char* input, struct list* jobs, int* background, int* nextID, pid_t pgID) 
{
	struct job* job = (struct job*)malloc(sizeof(struct job));
	job->cmd = input;
	job->id = *nextID;
	job->status = -1;
	job->bg = *background;

	if((job->pid = fork()) < 0)
		err_sys("fork error");
	else if(job->pid == 0) {//child process
		setpgid(0,0);
		job->pgid = getpgid(job->pid);

		//ignore CTRL-C, but handle CTRL-Z
		signal(SIGINT, SIG_IGN);
		signal(SIGTSTP, SIG_DFL);
		
		free(job);
		execvp(argv[0], argv);
		err_ret("Couldn't execute: %s", input);
		exit(EXIT_FAILURE);
	}
	
	if((setpgid(job->pid, job->pid)) != 0)
		err_sys("setpgid error\n");
	
	job->pgid = getpgid(job->pid);

	//make node with job data
    NodePtr node = createNode(job);
	
	if(job->bg) {//background flag
		printf("[%d] %d\t%s\n", job->id, job->pid, job->cmd);
		//add node to job linked list
		addAtRear(jobs, node);
		waitpid(job->id, &job->status, WNOHANG);
	}
	else {
		//foreground process
		tcsetpgrp(STDIN_FILENO, job->pid); 
		if((waitpid(job->pid, &job->status, WUNTRACED)) < 0) {
			free(job->cmd);
			free(job);
		}else{
			if(WIFSTOPPED(job->status)){
				//catch stopped jobs, remove from list
				job->isRunning = "Stopped";
				removeNode(jobs, node);
				printf("\n[%d]\t%d\tStopped\t%s\n", job->id, job->pid, job->cmd);
			}
		}
		tcsetpgrp(STDIN_FILENO, pgID);
	}	
}

void printAllJobs(int flag, struct list* jobs){
	struct node* node = jobs->head;
	if(!isEmpty(jobs)){
		while(node !=NULL){
			struct job* job = (struct job*)node ->obj;
			//quality-of-life vars for printing
			int* status;
			int id = job->id;
			int pid = job->pid;
			char* running;
			char* cmd = job->cmd;

			if(waitpid(job->pid, &status, WNOHANG) == 0){
				if(flag == 1){
					job->isRunning = "Running"; //running processes
					running = job->isRunning;
					printJobs(id, pid, running, cmd);
				}
			}else{
				job->isRunning = "Done"; //completed processes
				running = job->isRunning;
				printJobs(id, pid, running, cmd);
				removeNode(jobs, node); //can be removed from linked list after reporting
				free(job);
			}
			node = node->next;
		}
	}
}

void printJobs(int id, int pid, char* running, char* cmd){
	printf("[%d] %d\t%s\t%s\n", id, pid, running, cmd); //formatted output
}

void changeDir(struct passwd* pwuid, char** argv, char* dir)
{
	if(argv[1] == NULL) { //no input
		chdir(pwuid->pw_dir);
		getcwd(dir, MAX_LENGTH);
	}else if(strcmp(argv[1], "~") == 0){ //home directory
		chdir(pwuid->pw_dir);
	}
	else {
		if((chdir(argv[1])) != 0)
			printf("%s: No such file or directory\n", argv[1]);
		else {
			getcwd(dir, MAX_LENGTH);
		}
	}
}

void printHistory(void)
{
	HIST_ENTRY **hist = history_list();
	HISTORY_STATE* myhist = history_get_history_state();
	
	int i = 0;
	for(i = 0; i < myhist->length; i++){
		printf("%d %s\n", i, hist[i]->line);
	}
}

int isEmptyCommand(char* input)
{
	int len = strlen(input);
	char* inputString = strtok(input, "\t\n");

	//check for length of 0 or null test string
	if(len == 0 || inputString == NULL){
		return 1;
	}

	return 0;
}


void removeWhiteSpace(char* in)
{
	int i = strlen(in) - 1;
	while(isspace(in[i])) {
		in[i] = '\0';
		i--;
	}
}

int parseAmpersand(char* in)
{
	int inputSize = strlen(in);
	int i = 0;

	if(strchr(in, '&') != NULL) {
		while(i < inputSize) {
			if(in[i] == '&') {
				in[i] = '\0';
				break;
			}
			i++;
		}
		removeWhiteSpace(in);

		return 1;
	}else{
		return 0;
	}

}


void parse(char* input, int* background, char* args[])
{	
	*background = parseAmpersand(input);
	args[0] = strtok(input, " \t\n");
	
	int i = 1;
	while((args[i] = strtok(NULL, " \t\n")) != NULL && i < MAX_ARGS){
		i++;
	}
}

		
void version(int argc, char** argv)
{
	if(argc != 1) {
        if(strcmp(argv[1], "-v") != 0) { //anything besides -v
            printf("Usage: ./mydash OR ./mydash -v\n");
            exit(EXIT_SUCCESS);
        }
        else { //-v flag found
            const char* versionInfo = git_version(); //finds version
            printf("mydash: Version 1: Revision %s (author: Noah Rudin noahrudin994@u.boisestate.edu)\n", versionInfo);
            exit(EXIT_SUCCESS);
        }
    }
}


int equals(const void *obj, const void *other)
{
	int *o1 = (int *) obj;
	int *o2 = (int *) other;
	return *o1 == *o2;
}

char *toString(const void *obj)
{
	const int MAX_DIGITS = 10; /* maximum digits in an int */
	char *buf;
	buf = (char *) malloc(sizeof(char)*MAX_DIGITS);
	return buf;
}

void freeObject(void *obj)
{
    free(obj);
}

char* generatePrompt(void){
    char* ret = getenv("USER"); //creates mydash prompt 
    char* host = getenv("HOSTNAME");
    host = strtok(host, ".");
    strcat(ret, "@"); //concatenates username and hostname
    strcat(ret, host);
    char* dash_prompt = getenv("DASH_PROMPT"); //custom dash prompt
    if(dash_prompt != NULL)
        strcat(ret, dash_prompt);
    else{
        strcat(ret, " > ");
    }
    return ret;
}


void signalHandler(int signo)
{
	if (signo == SIGINT) { //checks for CTRL-C
	printf("\n");
    siglongjmp(SIGINT_BUF, 1); //restores prompt image
  }
}
