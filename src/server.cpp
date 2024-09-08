#include <bits/stdc++.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
using namespace std;

std::vector<int> clients;
void *handleClient(void *clientSocketPtr);
void broadcast_message(const char *message);

int main()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1)
    {
        cerr << "Socket creation failed " << endl;
        return -1;
    }

    sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    if (bind(server_socket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        cerr << "Binding Failed" << endl;
        close(server_socket);
        return -1;
    }

    if (listen(server_socket, 5) == -1)
    {
        cerr << "Listening failed " << endl;
        close(server_socket);
        return -1;
    }

    cout << "Listening on port 8080" << endl;

    while (1)
    {

        sockaddr client_addr;

        socklen_t client_addr_len = sizeof(client_addr);

        int clientSocket = accept(server_socket, (sockaddr *)&client_addr, &client_addr_len);

        if (clientSocket == -1)
        {
            cerr << "Failed To Accept Connection " << endl;
            continue;
        }

        cout << "Client Connected" << endl;

        pthread_t thread_id;

        pthread_create(&thread_id, nullptr, handleClient, (void *)&clientSocket);
        pthread_detach(thread_id);
    }

    close(server_socket);
    return 0;
}

void *handleClient(void *client_socket_ptr)
{

    int clientSocket = *(int *)client_socket_ptr;
    clients.push_back(clientSocket);

    char buffer[1024];

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));

        int bytesRecv = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesRecv <= 0)
        {
            cerr << "CLIENT disconnected or error " << endl;
            close(clientSocket);
            break;
        }

        cout << "Client :: " << buffer << endl;

        broadcast_message(buffer);
    }

    return nullptr;
}

void broadcast_message(const char *message)
{
    for (int client_socket : clients)
    {
        send(client_socket, message, strlen(message), 0);
    }
}