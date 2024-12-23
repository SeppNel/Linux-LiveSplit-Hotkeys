#include <arpa/inet.h>
#include <iostream>
#include <netinet/tcp.h>

using std::cout;
using std::string;

#define PORT 16834

int initSocket(const string &addr, const uint port);
void SendPacket(const int &socketFd, const string &s);