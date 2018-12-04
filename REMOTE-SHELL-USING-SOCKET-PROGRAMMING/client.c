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
#include <arpa/inet.h>

#include "util.h"

#define MAX_LINE  1024
#define END_TAG   "[SVR-ENDING]"
#define SVR_PORT  12345

int main()
{       
    int sd;  
    struct sockaddr_in addr;
    char line[MAX_LINE];   
    char back[MAX_LINE];
    int shouldrun = 1;
    char* tmp;
    int len;
    
    // Read the server host
    printf("Enter the host: ");
    gets(line);

    
    // create the socket               
    if ( ( sd = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
    {
        printf( "Error socket()\n" );
        exit( 1 );
    }    
           
    // connect to the server    
    memset( &addr, 0, sizeof( addr ) );
    addr.sin_family = AF_INET;
    addr.sin_port = htons( SVR_PORT );
    inet_aton(line, &(addr.sin_addr));
        
    if (connect(sd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {      
        printf( "Error connect\n" );
        exit( 1 );
    }
    
    while (shouldrun)
    {
        // get input from stdin
        printf("netsh> ");
        fflush(stdout);
        gets(line);
        
        strcpy(back, line);
        // the line is blank
        if ( (tmp = strtok(back, " \t\r\n")) == NULL)
            continue;
        
        if (strcmp(tmp, "quit") == 0)
            shouldrun = 0;
        
        // write the line to server
        write_line(sd, line);
        
        // read result from the server until END_TAG
        while (read_line(sd, line) >= 0)
        {
            if (strcmp(line, END_TAG) == 0)
                break;
            printf("%s\n", line);
        } 
    }
    
    return 0;
    
}
