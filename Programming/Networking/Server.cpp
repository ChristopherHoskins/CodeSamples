#include "Server.h"

SOCKET Server::CreateSocket(int protocol)
{
  SOCKET result = INVALID_SOCKET;

  int type = SOCK_DGRAM;

  if (protocol == IPPROTO_TCP)
    type = SOCK_STREAM;

  result = socket(AF_INET, type, protocol);

  return result;
}

sockaddr_in* Server::CreateAddress(char* ip, int port)
{
  sockaddr_in* result = (sockaddr_in*)calloc(sizeof(*result), 1);
	result->sin_family = AF_INET;
	result->sin_port = htons(port);

	if (ip == NULL)
		result->sin_addr.S_un.S_addr = INADDR_ANY;
	else
		inet_pton(result->sin_family, ip, &(result->sin_addr));

	return result;
}

int Server::BindAddress(SOCKET sock, sockaddr_in* address)
{
  int size = sizeof(sockaddr_in);
	int result = bind(sock, (sockaddr*)address, size);

	if (result == SOCKET_ERROR)
		return GetLastError();

	return 0;
}

int Server::SendToAddress(SOCKET sock, char* buffer, int bytes, sockaddr_in* destination)
{
  int result = sendto(sock, buffer, bytes, 0,
                     (sockaddr*)destination,
                     sizeof(sockaddr_in));

	if (result == SOCKET_ERROR)
		return -1;
	else
		return result;
}

int Server::ReceiveFrom(SOCKET sock, char* buffer, int maxBytes)
{
  sockaddr sender;
	int size = sizeof(sockaddr);

	int bytes = recvfrom(sock, buffer, maxBytes, 0, &sender, &size);

  if (bytes == SOCKET_ERROR)
  {
    int error = WSAGetLastError();
		return -1;
  }

	// Sender’s IP address is now in sender.sa_data
	return bytes;
}

void Server::Close(SOCKET sock)
{
  closesocket(sock);
}