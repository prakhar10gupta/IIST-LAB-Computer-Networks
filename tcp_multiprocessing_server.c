#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // include this in case sleep() or close() doesn't work
#define SERV_TCP_PORT 9877
#define MAXLINE 512
// #define CONN_LIMIT 2 // Number of clients connected at a time

/*
* Read a stream socket one line at a time, and write each line back
* to the sender.
*
* Return when the connection is terminated.
*/

void rev_string(line)
char line[MAXLINE];
{
    int size = strlen(line);
    char temp;
    for (int ii = 0; ii < (size/2); ii++) 
    {
        temp = line[ii];
        line[ii] = line[size-ii-1];
        line[size-ii-1] = temp;
    }
}


void interact(sockfd)
int sockfd;
{
    char line[MAXLINE];
    bzero((char *) line, sizeof(line));
    int n = recv(sockfd, line, MAXLINE, 0);
    if (n == 0)
        return; /* connection terminated */
    else if (n < 0)
        perror("interact: readline error");
    else printf("Message received: %s\n", line);
    
    sleep(2); // to model the "busy" nature of server
    
    rev_string(line); // the service
    
    if (send(sockfd, line, n, 0) != n)
    perror("interact: writen error");
    else printf("Message sent: %s\n", line);
}


int main()
{
int sockfd, newsockfd, clilen, childpid;
struct sockaddr_in cli_addr, serv_addr;

if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    perror("server: can't open stream socket");
else printf("Socket created\n");

bzero((char *) &serv_addr, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
serv_addr.sin_port = htons(SERV_TCP_PORT);
if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    perror("server: can't bind local address");
else printf("Binding successful\n");

listen(sockfd, 5);

// Wait for connections from client processes. Example of a multiprocessing server.
//for (int ii = 0; ii < CONN_LIMIT; ii++) 
for (; ;)
{
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
        perror("server: accept error");
    else printf("Connection accepted from a client\n");

    if ((childpid = fork()) < 0)
        printf("server: fork error");

    else if (childpid == 0) // child process
        {
            interact(newsockfd);
            exit(0); // terminates the child process and all of its sockets
        }
    else close(newsockfd); // as parent doesn't require this
}
close(sockfd);
return 0;
}

