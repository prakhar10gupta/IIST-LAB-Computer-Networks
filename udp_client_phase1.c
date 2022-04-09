# include <arpa/inet.h>
# include <sys/socket.h>
# include <stdio.h>
# include <string.h>

# define PORT 5000
# define BUFSIZE 32
# define SERVER_ADDR "127.0.0.1"
 
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

    // Initializing server's address variables: IP and port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_aton(SERVER_ADDR, &server_addr.sin_addr);

    int sent_bytes, recv_bytes, flag = 0; 
    int size_server_addr = sizeof(server_addr);
    char send_buf[BUFSIZE], recv_buf[BUFSIZE];
    
    while(1)
    { 
        printf("\nEnter message to send: ");
        scanf("%s", send_buf);

        // Sending packets to server
        sent_bytes = sendto(sockfd, &send_buf, sizeof(send_buf), flag, (struct sockaddr*)&server_addr, size_server_addr);
        printf("Message sent: %s\tTotal size: %ld\tReceiver's IP: %s\n", send_buf, sizeof(send_buf), inet_ntoa(server_addr.sin_addr));

        // Receiving packets from the server
        recv_bytes = recvfrom(sockfd, &recv_buf, sizeof(recv_buf), flag, (struct sockaddr*)&server_addr, &size_server_addr);
        printf("Message received: %s\tTotal size: %ld\tSender's IP: %s\n", recv_buf, sizeof(recv_buf), inet_ntoa(server_addr.sin_addr));
    }
    return 0;
}