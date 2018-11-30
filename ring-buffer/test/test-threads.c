#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ring.h>
#include <pthread.h>


static struct{
    //struct 
    int numThreads;
    int messages;
} args;

void * logs(void* id){
    //log messages for the ring log
    int i=0;
    long threadID = (long)id;
    for(i=0;i<args.messages;i++){
        char output[75];
        sprintf(output,"[Thread %ld] log message %d", threadID, i);
        log_msg(output);
    }
    printf("thread %ld finishing\n", threadID);
    return NULL;
}

int main(int argc, char *argv[]){
    //parses arguments and creates the pthreads
    pthread_t threadArray[args.numThreads];
    long i = 0;
  
if(argc<2||argc>3){
	printf("Usage: ./grader <numthreads> <mesages>.\nSample input: ./grader 4 10\n");
	exit(0);
	}
    init_buffer();
    args.numThreads=atoi(argv[1]);
    args.messages=atoi(argv[2]);
    for(i=0; i<args.numThreads; i++){
        //creates pthreads for desired numThreads
        printf("test-threads: thread %ld starting to log %d messages\n", i, args.messages);
        pthread_create(&threadArray[i], NULL, logs, (void *)i );
       
    }
    for(i=0; i<args.numThreads; i++){
        pthread_join(threadArray[i], NULL);
        
    }

printf("Sleeping for %d seconds to let backup thread finish\n", alarm_interval +2);
sleep(alarm_interval+2);
exit(EXIT_SUCCESS);
}
