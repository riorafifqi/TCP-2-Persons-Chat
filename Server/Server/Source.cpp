#include <iostream>
#include <stdlib.h>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#define MAX 1024
#define PORT 6666

using namespace std;

void chat(SOCKET client, SOCKET client2)
{
    char buff[MAX];
    string input;
    SOCKET clients_temp[2] = { client,client2 };
    int turn = 0;

    while (true) {
        memset(buff, 0, MAX);   // clear buff before receiving
        
        // set turn
        turn = 1;
        send(clients_temp[0], (const char*)&turn, sizeof(turn), 0);
        turn = 0;
        send(clients_temp[1], (const char*)&turn, sizeof(turn), 0);
           
        // read the message from client and copy it in buffer
        recv(clients_temp[0], (char*)&input, sizeof(input), 0);
            
        // Send that input to other client
        send(clients_temp[1], input.c_str(), sizeof(input), 0);
        
        // end turn
        turn = 0;
        send(clients_temp[0], (const char*)&turn, sizeof(input), 0);
        turn = 1;
        send(clients_temp[1], (const char*)&turn, sizeof(turn), 0);
        
        // read the message from client and copy it in buffer
        recv(clients_temp[1], (char*)&input, sizeof(input), 0);

        // Send that input to other client
        send(clients_temp[0], input.c_str(), sizeof(input), 0);
        
        // if input contains "exit", server exit and chat ended.
        //if (input == "exit") {
        //    cout << "Server Exit" << endl;
        //    break;
        //}
    }
}

int main()
{
    SOCKET server, client[2]{};
    sockaddr_in servaddr, cliaddr[2];

    // Initialize winsock
    static WSADATA wsaData;
    int wsatest = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsatest)
        exit(1);

    // Create socket
    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == INVALID_SOCKET) {
        cout << "Socket creation failed" << endl;
        exit(0);
    }
    else
        cout << "Socket succesfully created" << endl;
    memset(&servaddr, 0, sizeof(servaddr));

    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.S_un.S_addr = INADDR_ANY;     // Assign to
    //all available interface, not just localhost
    servaddr.sin_port = htons(PORT);

    // Bind socket and check if socket succesfully binded
    if ((bind(server, (sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        cout << "Socket binding failed" << endl;
        exit(0);
    }
    else
        cout << "Socket succesfully binded" << endl;

    // listen and check if server is ready for listening
    if ((listen(server, 2)) != 0) {
        cout << "Listening failed" << endl;
        exit(0);
    }
    else
        cout << "Server is listening" << endl;
    
    // Accept client socket
    for (int i = 0; i < 2; i++)
    {
        int lenClient = sizeof(cliaddr);
        client[i] = accept(server, (sockaddr*)&cliaddr, &lenClient);
        if (client[i] == INVALID_SOCKET) {
            cout << "Server accept failed" << endl;
            exit(0);
        }
        else
            cout << "Server accepting client" << endl;
    }

    // Function for chatting between client and server
    chat(client[0], client[1]);

    // After chatting close the socket
    closesocket(server);

    // Close Winsock
    WSACleanup();
}