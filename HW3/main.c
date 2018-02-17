#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>

#define NUM_OF_THREADS (2)

#define PRINT_THREAD_IDENTIFIER()		printf("[PID:%ld] [TTID:%ld] [Pthread_TID:%lu] ",getpid(), pthread_self(), syscall(SYS_gettid))
#define LOG_THREAD_IDENTIFIER() 		LOG("[PID:%ld] [TTID:%ld] [Pthread_TID:%lu] ",getpid(), pthread_self(), syscall(SYS_gettid))


#define LOG_FILENAME "Homework3.log"
#define LOG_INIT(filename)	FILE *fp = fopen(filename,"a+")
#define GET_LOG_HANDLE() fp
#define LOG(format,...) LOG_THREAD_IDENTIFIER(); fprintf(fp, format,__VA_ARGS__)
#define LOG_CLOSE() fclose(fp)

struct threadParams{

    pthread_t threadId;
    char *info;
    char *filename;
};

sem_t gotSignal_sem;

void* callBack_thread0(void* params)
{
	struct threadParams *inParams = (struct threadParams*)params;
	pthread_t self_pthreadId = pthread_self();
	pid_t process_id = getpid();
	pid_t linux_threadID = syscall(SYS_gettid);

	LOG_INIT(inParams->filename);
	if(NULL == GET_LOG_HANDLE)
		printf("File open error\n");

	LOG("Setup of Thread0 done\n");


	/* TO DO - Add functions for parsing Valentines.txt */

	LOG_PRINT("Waiting for SIGUSR.\n");
	PRINT_THREAD_IDENTIFIER();
	printf("Waiting for SIGUSR.\n");

	sem_wait(&gotSignal_sem);

	LOG("Exiting Thread 0\n");
	PRINT_THREAD_IDENTIFIER();
	printf("Exiting thread 0.\n");

	if(NULL != GET_FILE_HANDLE())
		LOG_CLOSE();

}

struct thread_cleanup{

	FILE *fp;
	void **heapMemArray;

};


void thread1_cleanup(void *arg)
{
	/* We need to clear the dynamic memory and file pointers */
	struct thread_cleanup *cleanup_mem = (struct thread_cleanup*)arg;

	if(NULL != cleanup->fp)
	{
		LOG_CLOSE(cleanup->fp);
		fp = NULL;
	}

	/* TO DO -Free any heap memory */

}

void* callBack_thread1(void* params)
{
	struct threadParams *inParams = (struct threadParams*)params;
	pthread_t self_pthreadId = pthread_self();
	pid_t process_id = getpid();
	pid_t linux_threadID = syscall(SYS_gettid);
	pthread_t self = pthread_self();

	LOG_INIT(inParams->filename);
	if(NULL == GET_LOG_HANDLE)
		printf("File open error\n");

	
	LOG("Setup of Thread1 done\n");

	//LOG("Registering cleanup function\n");

	//struct thread_cleanup cleanup_struct = { .fp = GET_FILEHANDLE() ; .headMemArray = NULL ; }
	//pthread_cleanup_push(thread1_cleanup,(void*)&cleanup_struct);

	/* TO DO - Create and start 100ms timer with callback which prints CPU utilization */

	LOG_PRINT("Waiting for SIGUSR.\n");
	PRINT_THREAD_IDENTIFIER();
	printf("Waiting for SIGUSR.\n");

	sem_wait(&gotSignal_sem);

	LOG("Exiting Thread 1\n");
	PRINT_THREAD_IDENTIFIER();
	printf("Exiting thread 1.\n");

	if(NULL != GET_FILE_HANDLE())
		LOG_CLOSE();
}

void signal_handler(int signal)
{
	switch (signal):
	case SIGUSR1:
		LOG_PRINT("SIGUSR1 signal\n");
		sem_post(&gotSignal_sem);
		break;
	case SIGUSR2:
		LOG_PRINT("SIGUSR2 signal\n");
		sem_post(&gotSignal_sem);
		break;
	default:
		LOG_PRINT("Invalid signal\n");
		break;

}

int register_signalHandler(struct sigaction *sa,void (*handler)(int))
{
	sa->sa_handler = handler;

	sa->sa_flags = SA_RESTART;

	sigfillset(&sa->sa_mask);

	int ret_error = 0;
	
	if (sigaction(SIGUSR1, sa, NULL) == SIG_ERR) 
	{
		ret_error++;
		PRINT_THREAD_IDENTIFIER();
		printf("Cannot handle SIGUSR1.\n");
		LOG_PRINT("Cannot handle SIGUSR1.\n");
	}

	if (sigaction(SIGUSR2, sa, NULL) == SIG_ERR) 
	{
		ret_error++;
		PRINT_THREAD_IDENTIFIER();
		printf("Cannot handle SIGUSR2.\n");
		LOG_PRINT("Cannot handle SIGUSR2.\n");
	}

	return ret_error;
}

int main()
{
    pthread_t p_threads[2];
	struct sigaction sa;
    int ret;
    struct threadParams thread_info[2];

	sem_init(&gotSignal_sem,0,0);
    
	LOG_INIT(LOG_FILENAME);
	if(NULL == GET_LOG_HANDLE())
		PRINT_THREAD_IDENTIFIER();

	LOG_PRINT("Log initialized.\n");
    
	register_signalHandler(&sa,signal_handler);

	thread_info[0].threadId 	= 0;
    thread_info[0].info 	 	= "Thread0";
	thread_info[0].filename 	= LOG_FILENAME;

    thread_info[1].threadId		= 1;
    thread_info[1].info	 		= "Thread1";    
	thread_info[1].filename 	= LOG_FILENAME;

	LOG_PRINT("Creating children Threads.\n");	

	ret = pthread_create(&p_threads[0], NULL, callBack_thread1, (void*)&thread_info[0]);
	if(ret != 0)
	{
		LOG("Cannot create child thread 0\n");
		return 1;
	}

	ret = pthread_create(&p_threads[1], NULL, callBack_thread2, (void*)&thread_info[1]);
	if(ret != 0)
	{
		LOG("Cannot create child thread 1\n");
		return 1;
	}


    LOG_PRINT("Thread created successfully\n");
	
	/* Waiting on child threads to complete */	
	ret = pthread_join(p_threads[0],NULL);
	if(0 != ret)
	{
		LOG("Pthread JOIN error\n"); printf("Join Error Thread 0\n");
		return 1;
	}
	
	ret	= pthread_join(p_threads[1],NULL);
	if(0 != ret)
	{
		LOG("Pthread JOIN error\n"); printf("Join Error Thread 0\n");
		return 1;
	}

    
	sem_destroy(&gotSignal_sem);
	printf("GoodBye!!\n");
	
    return EXIT_SUCCESS;
}
