#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

#define SERV_TCP_PORT 9877
#define SERV_HOST_ADDR "127.0.0.1" /* host addr for server replace it 
with the server’s IP address*/
#define MAXLINE 512

/*
* Read the contents of the FILE *fp, write each line to the
* stream socket (to the server process), then read a line back from
* the socket and write it to the standard output.
*
* Return to caller when an EOF is encountered on the input file.
*/
void interact(fp, sockfd)
register FILE *fp;
register int sockfd;
{
int n;
char sendline[MAXLINE], recvline[MAXLINE + 1];
struct timeval send_t, recv_t;
double send_t_us, recv_t_us, rtt, avg_rtt;

for (int ii = 0; ii < 1; ii++)
{
    printf("Enter the string to be sent to the server: ");
    fgets(sendline, MAXLINE, fp);
    sendline[strcspn(sendline, "\n")] = 0;
    n = strlen(sendline);
    printf("Sending '%s' message of %d bytes to server at %s\n",sendline, n,SERV_HOST_ADDR);

    if (send(sockfd, sendline, n,0) != n)
    perror("interact: writing error on socket");
    else
        {
            gettimeofday(&send_t, NULL);
            send_t_us = send_t.tv_sec*1e6 + send_t.tv_usec;
        }
    /*
    * Now read a line from the socket and write it to
    * our standard output.
    */
    n = recv(sockfd, recvline, MAXLINE,0);
    if (n < 0)
        perror("interact: readline error");
    else
        {
            gettimeofday(&recv_t, NULL);
            recv_t_us = recv_t.tv_sec*1e6 + recv_t.tv_usec;   
        }
    recvline[n] = 0; /* null terminate */

    printf("Received From Server: ");
    fputs(recvline, stdout);

    rtt = recv_t_us - send_t_us;
    //avg_rtt += rtt;
    printf("\nRound trip time: %f\n", rtt);
}
//printf("\nAverage round trip time: %f\n", avg_rtt);
}

/*
* Example of client using TCP protocol.
*/
int main()
{
int sockfd;
struct sockaddr_in serv_addr;

bzero((char *) &serv_addr, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
serv_addr.sin_port = htons(SERV_TCP_PORT);
/*
* Open a TCP socket (an Internet stream socket).
*/
if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
perror("client: can't open stream socket");
/*
* Connect to the server.
*/
if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    perror("client: can't connect to server");
else
    perror("Connected to the server\n");

interact(stdin, sockfd); /* do it all */

close(sockfd);
return 0;
}
