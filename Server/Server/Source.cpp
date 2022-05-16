#include <iostream>
#include <WinSock2.h>
#include <stdlib.h>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#define MAX 4096
#define PORT 6666
#define SA struct sockaddr

using namespace std;

void chat(SOCKET client)
{
    char buff[MAX];
    string input;

    while (true) {
        memset(buff, 0, MAX);   // clear buff before receiving
        
        // read the message from client and copy it in buffer
        recv(client, buff, sizeof(buff), 0);

        // print buffer which contains the client contents
        cout << "Client: " << buff << endl;
        cout << "Server: ";
        
        // Asking input for server message
        cin >> input;

        // Send that input to client
        send(client, input.c_str(), sizeof(input) + 1, 0);

        // if input contains "exit", server exit and chat ended.
        if (input == "exit") {
            cout << "Server Exit" << endl;
            break;
        }
    }
}

int main()
{
    SOCKET sockfd, client;
    sockaddr_in servaddr, cliaddr;

    // Initialize winsock
    static WSADATA wsaData;
    int wsatest = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsatest)
        exit(1);

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        cout << "Socket creation failed" << endl;
        exit(0);
    }
    else
        cout << "Socket succesfully created" << endl;
    memset(&servaddr, 0, sizeof(servaddr));

    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.S_un.S_addr = INADDR_ANY;     // Assign to all available interface, not just localhost
    servaddr.sin_port = htons(PORT);

    // Bind socket and check if socket succesfully binded
    if ((bind(sockfd, (sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        cout << "Socket binding failed" << endl;
        exit(0);
    }
    else
        cout << "Socket succesfully binded" << endl;

    // listen and check if server is ready for listening
    if ((listen(sockfd, 5)) != 0) {
        cout << "Listening failed" << endl;
        exit(0);
    }
    else
        cout << "Server is listening" << endl;
    
    // Accept client socket
    int lenClient = sizeof(cliaddr);
    client = accept(sockfd, (sockaddr*)&cliaddr, &lenClient);
    if (client == INVALID_SOCKET) {
        cout << "Server accept failed" << endl;
        exit(0);
    }
    else
        cout << "Server accepting client" << endl;

    // Function for chatting between client and server
    chat(client);

    // After chatting close the socket
    closesocket(client);

    // Close Winsock
    WSACleanup();
}