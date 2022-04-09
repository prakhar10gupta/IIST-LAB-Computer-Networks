#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>

# define PORT 5000
# define BUFSIZE 32

int main()
{
    // Creating socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    // Checking socket creation status
    if (sockfd < 0)
    {
        printf("Socket creation failed\n");
        return -1;
    }
    else
    {
        printf("Socket creation successful\n");
    }

    // Initializing address variables: IP and port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Binding address to the socket
    int bind_status = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Checking bind status
    if (bind_status < 0)
    {
        printf("Binding failed\n");
        return -1;
    }
    else
    {
        printf("Binding successful\n");
    }

    int recv_bytes, sent_bytes, flag = 0; 
    struct sockaddr_in client_addr;
    int size_client_addr = sizeof(client_addr);
    char recv_buf[BUFSIZE], send_buf[BUFSIZE];
    
    while(1)
    {
        

        // Receiving packets from a client
        recv_bytes = recvfrom(sockfd, &recv_buf, sizeof(recv_buf), flag, (struct sockaddr*)&client_addr, &size_client_addr);
        printf("Message received: %s\tTotal size: %ld\tSender's IP: %s\n", recv_buf, sizeof(recv_buf), inet_ntoa(client_addr.sin_addr));

        // Sending packets to the client
        for (int ii = 0; ii < sizeof(recv_buf); ii++)
            send_buf[ii] = recv_buf[ii];
        sent_bytes = sendto(sockfd, &send_buf, sizeof(send_buf), flag, (struct sockaddr*)&client_addr, sizeof(client_addr));
        printf("Message sent: %s\tTotal size: %ld\tReceiver's IP: %s\n", send_buf, sizeof(send_buf), inet_ntoa(client_addr.sin_addr));
    }
    return 0;
}