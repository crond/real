#include <stdio.h>
#include <stdarg.h>
#include <sched.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void sighandler(int signum){
    printf("SIgnal :%d caught. \n");
    if(signum == SIGSEGV)
       _exit(0);
    return;
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
    int min=0;
    if((min=sched_get_priority_min(ret)) < 0){
        printf("Error when getting min priority. Err: %d:%s\n",errno,strerror(errno));
        return -1;
    }    
    int max=0;
    if((max=sched_get_priority_max(ret)) < 0){
        printf("Error when getting min priority. Err: %d:%s\n",errno,strerror(errno));
        return -1;
    }    
    printf("\n For this policy, Priority values : Min:%d Max:%d\n",min,max);
    return 0;

}

