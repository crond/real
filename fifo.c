/*
 * File name    : fifo.c
 * Author       : crond
 * email        : cronddaemon@gmail.com
 * About File   : The output from this binary will do the following,
 *      1. print the current policy and its priority range
 *      2. change itself to FIFO policy and print its range
 * How to Run   :
 *      1. run this file binary output in system idle state
 *      2. observe the results
 *      3. run again with system's bit loaded state [ can use busy.c.out in
 *         'cpu' dir
 *      4. observe the results        
 */

#include <stdio.h>
#include <stdarg.h>
#include <sched.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_RUN_TIME_SEC        60
#define SLEEP_TIME_SEC           5 //should be less than MAX_RUN_TIME_SEC

int testSchedPeriod(){
    struct timeval fifotime; 
    int cnt=0;
    while( (cnt++) < (MAX_RUN_TIME_SEC/SLEEP_TIME_SEC)){
        gettimeofday(&fifotime,NULL);
        printf(" Start time: %02d:%02d usecDiff:%d\n",
                            fifotime.tv_sec,fifotime.tv_usec);
        sleep(SLEEP_TIME_SEC);
    } 
    return 0;
}
void sighandler(int signum){
    printf("SIgnal :%d caught. \n");
    if(signum == SIGSEGV)
       _exit(0);
    return;

}
int getrangeforschedpolicy(int policy){
    
    int min=0;
    if((min=sched_get_priority_min(policy)) < 0){
        printf("Error when getting min priority. Err: %d:%s\n",errno,strerror(errno));
        return -1;
    }    
    int max=0;
    if((max=sched_get_priority_max(policy)) < 0){
        printf("Error when getting min priority. Err: %d:%s\n",errno,strerror(errno));
        return -1;
    }    
    printf("\n For this policy, Priority values : Min:%d Max:%d\n",min,max);

}
int getschedpolicy(int pid){
    int ret =0;
    if((ret= sched_getscheduler(pid)) < 0){
        printf("Error when getting sched policy. Err: %d:%s\n",errno,strerror(errno));
        return -1;
    }    
    switch(ret){
        case 0:
            printf("Sched Type:Normal. Not running on any Priority  \n");
            break;
        case 1:
            printf("Sched Type:FIFO\n");
            break;
        case 2:
            printf("Sched Type:Round Robin Tim sharing Policy\n");
            break;
        case 3:
            printf("Sched Type:Batch. Not running on any Priority. \n");
            break;
       default:
            //Not supposed to come
            printf("Sched Type:Unknown\n");
            break;
    }
    return ret;
}
int main(int argc,char *argv[]){
    int ret=0;
    pid_t pid;
    
    signal(SIGSEGV,sighandler);
    if(argc == 2){
        char *buffer = (char *)malloc(strlen(argv[1]));
        bzero(buffer,sizeof(buffer));
        buffer = argv[1];
        sscanf(buffer,"%d",&pid);
    }
    else
        pid = getpid();
    printf("This proc pid:%d.Getting Scheduler Type for PID:%d. \n",getpid(),pid);
    int policy= getschedpolicy(pid);
    getrangeforschedpolicy(policy);
    testSchedPeriod();

    //changing to FIFO schedule policy
    struct sched_param schedPrity;
    schedPrity.sched_priority=10;
    errno =0;
    if((ret = sched_setscheduler(getpid(),SCHED_FIFO,(const struct sched_param *)&schedPrity)) < 0){
        printf("Error when setting sched algorithm. Err: %d:%s\n",errno,strerror(errno));
        return -1;
    
    }  
    policy= getschedpolicy(getpid());
    getrangeforschedpolicy(policy);
    testSchedPeriod();
    return 0;

}
