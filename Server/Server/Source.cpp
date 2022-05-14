#include <stdio.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <string.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Function designed for chat between client and server.
void func(SOCKET client)
{
    char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;) {
        memset(buff, 0, MAX);

        // read the message from client and copy it in buffer
        recv(client, buff, sizeof(buff), 0);
        // print buffer which contains the client contents
        printf("From client: %s\t To client : ", buff);
        memset(buff, 0, MAX);
        n = 0;
        // copy server message in the buffer
        while ((buff[n++] = getchar()) != '\n')
            ;

        // and send that buffer to client
        send(client, buff, sizeof(buff), 0);

        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

// Driver function
int main()
{
    SOCKET sockfd, client;
    int len;
    sockaddr_in servaddr, cli;

    // initialize winsock
    static WSADATA wsaData;
    int wsatest = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsatest)
        exit(1);

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    memset(&servaddr, 0, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.S_un.S_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    client = accept(sockfd, (SA*)&cli, &len);
    if (client == INVALID_SOCKET) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");

    // Function for chatting between client and server
    func(client);

    // After chatting close the socket
    closesocket(client);

    // Close Winsock
    WSACleanup();
}