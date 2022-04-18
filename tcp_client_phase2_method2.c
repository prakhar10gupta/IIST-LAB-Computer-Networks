#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // for using close()

#define PORT 5000
#define BUFFERSIZE 128
#define SERVER_ADDR "127.0.0.1"
#define N 10 // number of packets

int main()
{   
    struct sockaddr_in server_addr, client_addr;

    // Creating socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sock_fd<0)
    {
        printf("Socket creation failed\n");
        return -1;
    }
    else
    {
        printf("Socket creation successful\n");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_aton(SERVER_ADDR, &server_addr.sin_addr);
    // server_addr.sin_addr.s_addr = htonl(SERVER_ADDR); // What"s the problem in this?

    // Sending connection request to server
    int conn_status = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if (conn_status<0)
    {
        printf("Connection request failed\n");
        return -1;
    }
    else
    {
        printf("Connected to server successful\n");
    }
    
    // Defining the message to be sent
    char msg[BUFFERSIZE];
    int header;
    printf("Enter your roll number: ");
    scanf("%s", msg);

    char zero = '0';
    char send_buf[BUFFERSIZE], recv_buf[BUFFERSIZE];
    for(int ii = 0; ii < strlen(msg); ii++)
    {
        send_buf[ii+1] = msg[ii];
    }
    // strcpy(msg, "Hello_there!");
    int flag = 0;
    int num_bytes_sent, num_bytes_recv;
    int total_num_bytes_sent = 0;
    
    // Sending packets to server
    for (int ii = 0; ii < N; ii++)
    {
        header = ii;
        send_buf[0] = header + zero;
      
        int buf_size = strlen(send_buf);
        num_bytes_sent = send(sock_fd, &send_buf, buf_size, flag);
        printf("Message sent: %s  Total size: %d \n", send_buf, num_bytes_sent);
        total_num_bytes_sent += num_bytes_sent;
    }
    
    // Receiving response from server
    int recv_calls = 0;
    int total_num_bytes_recv = 0;

    while(1)
    {
    num_bytes_recv = recv(sock_fd, &recv_buf, sizeof(recv_buf), flag);
    printf("Message received: %s  Total size: %d \n", recv_buf, strlen(recv_buf));
    
    total_num_bytes_recv += strlen(recv_buf);
    recv_calls++;
    if (total_num_bytes_recv >= total_num_bytes_sent) 
        break;
    }
    
    printf("Total number of system recv calls placed: %d\n", recv_calls);

    // Close connection
    close(sock_fd);

    return 0;
}