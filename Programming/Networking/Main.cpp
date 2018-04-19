#include <iostream>
#include "Server.h"

#define IP "104.131.138.5"
#define PORT 8888
#define MAX_BUFFER 1500

int main(int argc, char** argv)
{
  WSADATA wsa;

  if (WSAStartup(MAKEWORD(2, 2), &wsa) == 0)
  {
    Server UDP_Server;

    SOCKET UDP_socket = UDP_Server.CreateSocket(IPPROTO_UDP);
    sockaddr_in* socket_address = UDP_Server.CreateAddress(IP, PORT);
    
    UDP_Server.BindAddress(UDP_socket, socket_address);

    UDP_Server.SendToAddress(UDP_socket, argv[0], strlen(argv[0]), socket_address);

    int bytesReceived = 0;
    char in[MAX_BUFFER];

    while (!bytesReceived)
      bytesReceived = UDP_Server.ReceiveFrom(UDP_socket, in, MAX_BUFFER);

    char target[MAX_BUFFER];
    strncpy_s(target, in, bytesReceived);
    std::cout << target << std::endl;

    UDP_Server.Close(UDP_socket);

    free(socket_address);

    WSACleanup();
  }
  else
    WSAGetLastError();

  return 0;
}

