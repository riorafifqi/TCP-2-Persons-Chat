#include <iostream>
#include <WinSock2.h>
#include <stdlib.h>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#define MAX 1024
#define PORT 6666

using namespace std;

void chat(int sockfd)
{
    char buff[MAX];
    string input;
    int n;
    for (;;) {
        // Get client input and send it to server
        cout << "You: ";
        cin >> input;
        send(sockfd, input.c_str(), sizeof(input), 0);

        // receive message and copy it to buffer
        memset(buff, 0, sizeof(buff));  // Clear buff before receiving
        recv(sockfd, buff, sizeof(buff), 0);
        cout << "Server " << buff << endl;  // print the buffer that contain server message

        // client exit if input include "exit"
        if (input == "exit") {
            cout << "Client Exit" << endl;
            break;
        }
    }
}

int main()
{
    SOCKET sockfd;
    struct sockaddr_in servaddr;

    // Initialize winsock
    static WSADATA wsaData;
    int wsatest = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsatest)
        exit(1);

    // Socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    memset(&servaddr, 0, sizeof(servaddr));

    // Assign structure
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

    // Connect to server socket
    if (connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // chat loop function
    chat(sockfd);

    // close socket
    closesocket(sockfd);

    // close Winsock
    WSACleanup();
}