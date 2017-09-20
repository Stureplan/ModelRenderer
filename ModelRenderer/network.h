#pragma once
#include <WinSock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define RECV_MAX 64

class Network
{
public:
	Network::Network(unsigned short port)
	{
		bindsock(port);
	}


	int Recieve(std::string &result)
	{
		sockaddr_in from;
		char buffer[RECV_MAX];

		int size = sizeof(from);
		int ret = recvfrom(sock, buffer, RECV_MAX, 0, (sockaddr*)&from, &size);

		if (ret > 0)
		{
			result = parse_string(buffer, RECV_MAX);
		}

		return ret;
	}

private:
	void bindsock(unsigned short port)
	{
		WSAStartup(MAKEWORD(2, 2), &data);
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (sock == INVALID_SOCKET)
		{
			// invalid socket: shit's fucked man
		}

		sockaddr_in add;
		add.sin_family = AF_INET;
		add.sin_addr.s_addr = htonl(INADDR_ANY);
		add.sin_port = htons(port);

		int ret = bind(sock, reinterpret_cast<SOCKADDR *>(&add), sizeof(add));
		if (ret < 0)
		{
			// bind failed
		}

		u_long nonblock = 1;
		ioctlsocket(sock, FIONBIO, &nonblock);
	}

	std::string parse_string(char* arr, unsigned int length)
	{
		std::string s = "";

		for (unsigned int i = 0; i < length; i++)
		{
			s.push_back(arr[i]);

			if (arr[i] == '\n')
			{
				break;
			}
		}

		return s;
	}

private:
	SOCKET sock;
	WSAData data;
}net(8888);