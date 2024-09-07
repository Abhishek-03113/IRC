#include<iostream> 
#include<bits/stdc++.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std; 


int main(){
    
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0); 

    sockaddr_in serverAddr; 

    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_port = htons(8080); 
    serverAddr.sin_addr.s_addr = INADDR_ANY; 

    connect(
        clientSocket, 
        (struct sockaddr*)&serverAddr,
        sizeof(serverAddr)
    ); 
    

    const char* message = "Hello Server"; 

    send(clientSocket, message, strlen(message), 0);

    return 0; 
}