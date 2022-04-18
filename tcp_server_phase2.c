#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // for using close()

#define PORT 5000
#define BUFFERSIZE 128

struct Message{
    int header; // for identifying sequence
    char data[BUFFERSIZE]; // for roll number
};

int main()
{
    struct sockaddr_in server_addr, client_addr;
    int server_addr_len = sizeof(server_addr);

    int num_bytes_recv, num_bytes_sent;
    struct Message send_buf, recv_buf;

    // Creating the Reqest socket 
    int req_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (req_sock_fd<0)
    {
        printf("Socket creation failed\n");
        return -1;
    }
    else
    {
        printf("Socket creation successful\n");
    }

    // Defining IP adddress and port for server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Binding IP address and port to the server socket
    int bind_status = bind(req_sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if (bind_status<0)
    {
        printf("Binding failed\n");
        return -1;
    }
    else
    {
        printf("Binding successful\n");
    }

    // Start listening to connection requests
    int backlog = 5; // backlog is not of concern here, as there is only one client
    listen(req_sock_fd, backlog); 
    printf("Listening to requests\n");

    // Accept request from a client
    int conn_sock_fd = accept(req_sock_fd, (struct sockaddr*)&client_addr, &server_addr_len); // server_addr_len is same as client_addr_len as they have been defined using same data type.
    
    if (conn_sock_fd<0)
    {
        printf("Connecting to client failed\n");
        return -1;
    }
    else
    {
        printf("Connected to client successful\n");
    }
    int flag = 0;
    
    // Receive packets from the client through the connection established
    for (int ii = 0; ii < 20; ii++) // Taking the max limit larger than number of messages sent
    {
        num_bytes_recv = recv(conn_sock_fd, &recv_buf, sizeof(recv_buf), flag);
        recv_buf.data[num_bytes_recv] = '\0';

        if (num_bytes_recv > 0) 
        {
            printf("Message received: %d%s  Total size: %d \n", recv_buf.header, recv_buf.data, num_bytes_recv);

            // Echo the packets back to the connected client
            send_buf = recv_buf;
            num_bytes_sent = send(conn_sock_fd, &send_buf, sizeof(send_buf), flag);
            printf("Message sent: %d%s  Total size: %d \n", send_buf.header, send_buf.data, num_bytes_sent);
        }
    }

    // Close the connections
    close(req_sock_fd);
    close(conn_sock_fd);

    return 0;
}
