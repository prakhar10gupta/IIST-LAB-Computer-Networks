#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // for using close()

#define PORT 5000
#define BUFFERSIZE 64
#define SERVER_ADDR "127.0.0.1"

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
        printf("Connected to server successfully\n");
    }

    char send_buf[BUFFERSIZE], recv_buf[BUFFERSIZE];
    
    // Defining the message to be sent
    printf("Enter the secret message: ");
    scanf("%s", send_buf);
    // strcpy(send_buf, "Hello there!");
    int flag = 0;

    // Sending packet to server
    int num_bytes_sent = send(sock_fd, send_buf, strlen(send_buf), flag);
    printf("Message sent: %s  Total size: %d \n", send_buf, num_bytes_sent);

    //Receiving response from server
    int num_bytes_recv = recv(sock_fd, recv_buf, sizeof(recv_buf), flag);
    printf("Message received: %s  Total size: %d \n", recv_buf, num_bytes_recv);

    //Close connection
    close(sock_fd);

    return 0;
}