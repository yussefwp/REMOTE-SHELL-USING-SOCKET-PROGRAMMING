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
#include <sys/wait.h>

#include "util.h"

#define MAX_LINE  1024
#define END_TAG   "[SVR-ENDING]"
#define SVR_PORT  12345

#define MAX_CMDS  128

// execute the client commands
void exe_client(int sd);

int main()
{
    int sd;
    struct sockaddr_in addr;
    struct sockaddr_in caddr;
    int csd;
    int addrlen;
    pid_t pid;
    
    // create the socket    
    if ( ( sd = socket( AF_INET, SOCK_STREAM, 0) ) == -1 )
    {
        perror( "Error socket\n");
        exit( 1 );
    }

    memset( &addr, 0, sizeof( addr ) );
    addr.sin_family = AF_INET;
    addr.sin_port = htons( SVR_PORT );  
    addr.sin_addr.s_addr = INADDR_ANY;    
    

    // bind the server socket
    if ( bind( sd, ( struct sockaddr *) &addr, sizeof( addr ) ) == -1)
    {
        printf( "Error bind\n" );
        exit( 1 );
    }  

    if ( listen( sd, 5 ) == -1 ) 
    {
        printf( "Error listen\n" );
        exit( 1 );
    }
    
    while (1)
    {     
        // a new client connected      
        addrlen = sizeof(caddr);
        csd = accept(sd, (struct sockaddr *) &caddr, &addrlen);
    
        if (csd != -1)
        {
            printf("new client enter\n");
            pid = fork();
            if (pid < 0)
            {
                printf("Error fork\n");
                exit(1);
            }
            if (pid == 0)
            {
                // child process
                close(sd);
                exe_client(csd);
                return 0;
            }
            // parent continue 
            close(csd);
        }
    }

    return 0;       
}
   
// execute the client commands
void exe_client(int sd)
{
    char line[MAX_LINE];
    char* args[MAX_CMDS];
    int numArgs = 0;
    pid_t pid;
    int status;
    
    while (1)
    {
        // read the command from the client
        if (read_line(sd, line) < 0)
            break;
        
        numArgs = 0;
        
        args[numArgs] = strtok(line, " \t\r\n");
        while (args[numArgs] != NULL) {
            numArgs++;          
            args[numArgs] = strtok(NULL, " \t\r\n");            
        }    
            
        if (strcmp(args[0], "quit") == 0)
            break;
            
        pid = fork();
        if (pid == -1)
            break;
        
        if (pid == 0)
        {
            // process which execute the command 
            close(STDOUT_FILENO);                         
            dup2(sd, STDOUT_FILENO);
            close(sd);
            execvp(args[0], args); 
            exit(1);
        }
        
        // wait for the process
        waitpid(pid, &status, 0);          
    
        // write ending tag
        write_line(sd, "");
        write_line(sd, END_TAG);
    }
    printf("client quit\n");
    close(sd);
}

