#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FILENAME "testfile"

unsigned char buffer[128];
int main()
{
    FILE *fd;
    int ret;
    struct timeval  start,end,diff;

    if( (fd= fopen(FILENAME,"r")) <= 0){
        perror("Error while OPening file. ");
        return 0;
    }
    ret =0;
    printf(" freadOut:\n");
    gettimeofday(&start,NULL);
    ret = fread(buffer, sizeof(buffer),1,fd);
//    printf("Buffer: %s \n",buffer);
    while(ret > 0){
//        memset(buffer,0,sizeof(buffer));
        ret = fread(buffer, sizeof(buffer),1,fd);
  //      printf("Buffer: %s \n",buffer);
    }        
    gettimeofday(&end,NULL);
    printf(" Start time: %02d:%02d\n",
                            start.tv_sec,start.tv_usec);
    printf(" End time  : %02d:%02d\n",
                            end.tv_sec,end.tv_usec);
    printf(" End time  : %02d:%02d\n",
                            end.tv_sec-start.tv_sec,end.tv_usec-start.tv_usec);
    return 0;
}
