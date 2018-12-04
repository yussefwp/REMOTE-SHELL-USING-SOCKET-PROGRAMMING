#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <unistd.h>     
#include <time.h>
                      
// read a line into buf, return -1 if failed
int read_line(int fd, char *buf)
{
    int sum = 0, r;
    do
    { 
        r = read(fd, buf + sum, 1);
        if (r <= 0)
            return -1;
        sum ++;
    } while (buf[sum-1] != '\n');
    buf[sum-1] = 0;
    return 0;
} 
     
// write a line into buf, append '\n', return -1 if failed
int write_line(int fd, char *buf)
{  
    int len = strlen(buf);
    int sum = 0, r;
    char tmp = '\n';
    
    while (sum < len)
    {
        r = write(fd, buf + sum, len - sum);
        if (r < 0)
            return -1;
        sum += r;
    }
    
    if (write(fd, &tmp, 1) < 0)
        return -1;
    return 0;    
}


