#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h> // for exit()
#include <string.h>

#define BUFFERSIZE 32
#define PORT 5000

struct Message{
    int header;
    char data[BUFFERSIZE];
};

int main()
{
    int sock; // creating the 'door'
    
    struct sockaddr_in s_server, s_client; // address of people who want to communicate
    
    int bind_status; // whether address completely specified
    
    int recv_bytes_num, send_bytes_num;
    int control_info = 0;

    struct Message send_msg, recv_msg;
   
    int s_client_len = sizeof(s_client);

    // Creating socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // protocol family, domain(connection_less), protocol name
    if (sock < 0)
    {
        printf("Socket creation failed.\n");
        exit(0);
    }
    else
    {
        printf("Socket created successfully with file descriptor: %d\n", sock);
    }

    // Initializing and binding socket
    s_server.sin_family = AF_INET;
    s_server.sin_port = htons(PORT);
    s_server.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY connects the port to all the local interfacesm useful when server is serving multiple channels.
    // I can use sin_addr(127.0.0.1) as well

    bind_status = bind(sock, (struct sockaddr*)&s_server, sizeof(s_server));
    if (bind_status < 0)
    {
        printf("Binding scoket failed.\n");
    }
    else
    {
        printf("Binding socket successful.\n\n");
    }

    while(1)
    {
        recv_bytes_num = recvfrom(sock, &recv_msg, sizeof(recv_msg), control_info, \
        (struct sockaddr*)&s_client, &s_client_len);

        printf("Number of bytes received: %d\tMessage sequence: %d\tMessage: %s\nReceived from IP: %s PORT: %d\n", \
        recv_bytes_num, recv_msg.header, recv_msg.data, inet_ntoa(s_client.sin_addr), s_client.sin_port);

        // Send back to client
        send_msg = recv_msg;
        send_bytes_num = sendto(sock, &send_msg, sizeof(send_msg), control_info, \
        (struct sockaddr*)(&s_client), s_client_len);

        printf("Number of bytes sent: %d\tMessage sequence: %d\tMessage: %s\n\n", send_bytes_num, send_msg.header, send_msg.data);
    }
    
    return 0;
}