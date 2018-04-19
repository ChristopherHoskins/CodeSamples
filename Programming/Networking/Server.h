#include <WinSock2.h>
#include <WS2tcpip.h>

class Server
{ 
  public:
    SOCKET CreateSocket(int protocol);
    sockaddr_in* CreateAddress(char* ip, int port);
    int BindAddress(SOCKET sock, sockaddr_in* address);
    int SendToAddress(SOCKET sock, char* buffer, int bytes, sockaddr_in* destination);
    int ReceiveFrom(SOCKET sock, char* buffer, int maxBytes);
    void Close(SOCKET sock);
};