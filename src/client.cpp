#include <iostream>
#include <bits/stdc++.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
using namespace std;

void *recieve_message(void *socket_ptr);
void send_message(int client_socket);
void welcome_message();

int main()
{
    welcome_message();

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket == -1)
    {
        cerr << "Socket Creation Failed";
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        cerr << "Connection To server Failed" << endl;
        close(client_socket);
        return -1;
    }
    cout << "Connected To the server" << endl;

    pthread_t thread_id;

    pthread_create(&thread_id, nullptr, recieve_message, (void *)&client_socket);

    cout << "ENTER YOUR MESSAGE TO SEND TO SERVER " << endl;

    send_message(client_socket);
    close(client_socket);
    return 0;
}

void *recieve_message(void *socket_ptr)
{

    int client_socket = *(int *)socket_ptr;
    char buffer[1024];

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));

        int bytesRecv = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytesRecv <= 0)
        {

            cerr << "Connection Closed or Error Occured" << endl;
            close(client_socket);

            pthread_exit(nullptr);
        }

        cout << "Server : " << buffer << endl;
    }

    return nullptr;
}

void send_message(int client_socket)
{
    char buffer[1024];
    while (1)
    {
        cin.getline(buffer, sizeof(buffer));

        if (send(client_socket, buffer, strlen(buffer), 0) == -1)
        {
            cerr << "Failed To send message " << endl;
            break;
        }
    }
}

void welcome_message()
{

    cout << R"(
    ███████╗██╗  ██╗██╗██████╗ ██╗██████╗ ██╗     ██████╗██╗  ██╗ █████╗ ████████╗
    ██╔════╝██║ ██╔╝██║██╔══██╗██║██╔══██╗██║    ██╔════╝██║  ██║██╔══██╗╚══██╔══╝
    ███████╗█████╔╝ ██║██████╔╝██║██║  ██║██║    ██║     ███████║███████║   ██║   
    ╚════██║██╔═██╗ ██║██╔══██╗██║██║  ██║██║    ██║     ██╔══██║██╔══██║   ██║   
    ███████║██║  ██╗██║██████╔╝██║██████╔╝██║    ╚██████╗██║  ██║██║  ██║   ██║   
    ╚══════╝╚═╝  ╚═╝╚═╝╚═════╝ ╚═╝╚═════╝ ╚═╝     ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝                                                                    
                                                                                
)" << endl;
}