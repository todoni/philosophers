#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void printids(const char *s)
{   
    pid_t pid;
    pthread_t tid;
    pid = getpid(); 
    tid = pthread_self(); 
    printf("%s pid %u tid %u (0x%x)\n", s, 
            (unsigned int)pid, (unsigned int)tid, (unsigned int)tid);
}

void * thr_fn(void *arg)
{
    printids("new thread: ");
    return((void *)0);
}

int main(void)
{
    int err;
	pthread_t	ntid;
    err = pthread_create(&ntid, NULL, thr_fn, NULL);
    if (err != 0)
        perror("can't create thread");
	//err = pthread_join(ntid, NULL);
    printids("main thread:");
    //sleep(1);           
    return (0);                
}
