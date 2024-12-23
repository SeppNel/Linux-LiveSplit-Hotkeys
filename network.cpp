#include "network.h"

int initSocket(const string &addr, const uint port) {
    int clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(addr.c_str());
    servaddr.sin_port = htons(port);
    int synRetries = 2; // Send a total of 3 SYN packets => Timeout ~7s
    setsockopt(clientsocket, IPPROTO_TCP, TCP_SYNCNT, &synRetries, sizeof(synRetries));

    if (clientsocket == -1) {
        cout << "Socket could not be created\n";
    }

    if (connect(clientsocket, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
        cout << "Connection with the server failed...\n";
        exit(0);
    } else {
        cout << "Connected to the server..\n";
    }

    return clientsocket;
}

void SendPacket(const int &socketFd, const string &s) {
    void const *buffer = s.c_str();
    uint n = s.size();

    send(socketFd, buffer, n, 0);
}