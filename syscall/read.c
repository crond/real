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
    int fd,ret;
    struct timeval  diff,start,end;

    if( (fd= open(FILENAME,O_RDONLY)) <= 0){
        perror("Error while OPening file. ");
        return 0;
    }
    ret =0;
    printf(" readOut:\n");
    gettimeofday(&start,NULL);
    ret = read(fd, buffer, sizeof(buffer));
    write(1,buffer,ret);
    while(ret > 0){
        bzero(buffer,sizeof(buffer));
        ret = read(fd, buffer, sizeof(buffer));
        write(1,buffer,ret);
    }        
    gettimeofday(&end,NULL);
    printf(" Start time: %02d:%02d\n",
                            start.tv_sec,start.tv_usec);
    printf(" End time  : %02d:%02d\n",
                            end.tv_sec,end.tv_usec);
    printf(" read diff time  : %02d:%02d\n",
                            end.tv_sec-start.tv_sec,end.tv_usec-start.tv_usec);

    return 0;
}
