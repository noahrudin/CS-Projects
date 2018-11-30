#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ring.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


static struct {
	int curr;
	char log[MAX_LOG_ENTRY][MAX_STRING_LENGTH];
} buff;

static void dump_buffer();
static bool init;
static void flag(int signum);


void init_buffer()
{
	printf("Initialize ring buffer\n");
	signal(SIGALRM, flag); //initializes handler

	int i;
	for(i = 0; i < MAX_LOG_ENTRY; i++) {
		buff.log[i][0]='\0';
	}
	alarm(alarm_interval); //run signal
	buff.curr = 0;

	init=false;
}

/**
 * Return the current timestamp (localtime) from the system.
 */
static char *getTimeString()
{
	time_t myTime = time(NULL); //this is a system call
	char *timeString = ctime(&myTime);
	timeString[strlen(timeString)-1] = '\0'; //erase the newline at the end
	return timeString;
}

void log_msg(char *entry)
{
	if (entry == NULL) {
		printf("Skipping null log entry!\n");
		return;
	}

	printf("Adding log entry into buffer\n");

	char *timeString = getTimeString();
	int idx = buff.curr % MAX_LOG_ENTRY;

	snprintf(buff.log[idx], MAX_STRING_LENGTH, "%s -- %s", timeString, entry);

	buff.curr++;

	if(init!=false){
		dump_buffer();
		init=false;

	}
}



/*
 * Right now this is just printing to the console. We want to change this to
 * write to a file (log_name) and we want to use signals to trigger the logging
 * event. This also needs to be fixed so that it prints the log messages in the
 * right order (from the oldest to the newest).
 *
 * This method should write all the current entries to disk in the right order
 * (from the oldest to the newest). We will use the constant log_name as the
 * name of the file.
 */
static void dump_buffer()
{

	FILE *file =fopen(log_name, "w");

	if(file==NULL){
		printf("Couldn't open file.\n");
		exit(0);

	}else{

		int i;
		for(i = 0; i < MAX_LOG_ENTRY; i++) {
			fprintf(file, "log %d: %s\n",i, buff.log[i]);
		}

	}
	fclose(file);
}


static void flag(int signum){

	fflush(stdout);
	signal(SIGALRM, flag);
	alarm(alarm_interval);
	init=true;

}
