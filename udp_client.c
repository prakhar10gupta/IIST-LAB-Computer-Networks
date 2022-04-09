#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for exit()

#define BUFFERSIZE 32
#define SERVER_ADDR "127.0.0.1"
#define PORT 5000
#define MSG "ping"
#define numofpkts 10

struct Message{
    int header;
    char data[BUFFERSIZE];
};

int main()
{
    int sock;

    struct sockaddr_in s_server;
    
    int recv_bytes_num, send_bytes_num;
    int control_info = 0;

    struct Message send_msg, recv_msg;
    strcpy(send_msg.data, MSG);
    
    int s_server_len = sizeof(s_server);
    
    struct timeval send_time, recv_time;
    double send_tv, recv_tv;

    // Creating client socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0)
    {
        printf("Socket not created.\n");
        exit(0);
    }
    else
    {
        printf("Socket created with file descriptor: %d\n", sock);
    }

    // Assigning server parameters for sending
    s_server.sin_family = AF_INET;
    s_server.sin_port = htons(PORT);
    if (!inet_aton(SERVER_ADDR, &s_server.sin_addr))
    {
        printf("Conversion of server ASCII address to IP network format failed.\n");
        exit(0);
    }
    else
    {
        printf("Conversion of server ASCII address to IP network format successful.\n\n");
    }

    // Collecting message from the user
    //printf("Please type the message to send (without space): ");
    //scanf(" %s ", send_msg.data);

// Round trip calculation 
    // Sending the message to server
    double temp_rtt = 0, avg_rtt = 0;
    for (int ii = 0; ii < numofpkts; ii++)
    {
        send_msg.header = ii;

        // Sending messsage
        send_bytes_num = sendto(sock, &send_msg, sizeof(send_msg), control_info, \
        (struct sockaddr*)&s_server, s_server_len);
    
        // Noting send time
        gettimeofday(&send_time, NULL);
        send_tv = send_time.tv_sec*1e6 + send_time.tv_usec;
        // printf("send time stamp: %f\n", send_tv);
        printf("Number of bytes sent: %d\tMessage sequence: %d\tMessage: %s\n", send_bytes_num, send_msg.header, send_msg.data);

        // Receiving message
        recv_bytes_num = recvfrom(sock, &recv_msg, sizeof(recv_msg), control_info, \
        (struct sockaddr*)(&s_server), &s_server_len);

        // Noting recv time
        gettimeofday(&recv_time, NULL);
        recv_tv = recv_time.tv_sec*1e6 + recv_time.tv_usec;
        // printf("recv time stamp: %f\n", recv_tv);
        printf("Number of bytes received: %d\tMessage sequence: %d\tMessage: %s\n", recv_bytes_num, recv_msg.header, recv_msg.data);

        // Calculating round trip time
        temp_rtt = recv_tv - send_tv;
        printf("Round-trip time of packet sequence %d: %f us\n\n", ii, temp_rtt);
        avg_rtt += temp_rtt;
    }
    avg_rtt /= numofpkts;
    printf("Average round-trip time: %f us\n\n", avg_rtt);


// Checking the order of messages  
    // Sending messsage
    int check_counter = 10;
    for (int send_counter = 0; send_counter < check_counter; send_counter++)
    {
        send_msg.header = send_counter;
        send_bytes_num = sendto(sock, &send_msg, sizeof(send_msg), control_info, \
        (struct sockaddr*)&s_server, s_server_len);
        printf("Number of bytes sent: %d\tMessage sequence: %d\tMessage: %s\n", send_bytes_num, send_msg.header, send_msg.data);
    }
    
    // Receiving message
    int recv_counter = 0; 
    printf("\n");
    while(recv_counter < check_counter)
    {
        recv_bytes_num = recvfrom(sock, &recv_msg, sizeof(recv_msg), control_info, \
        (struct sockaddr*)(&s_server), &s_server_len);
        printf("Number of bytes received: %d\tMessage sequence: %d\tMessage: %s\n", recv_bytes_num, recv_msg.header, recv_msg.data);
        recv_counter++;
    }
    
    return 0;
}