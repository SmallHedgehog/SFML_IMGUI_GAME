#include "Socket.h"


int Socket::nofSockets = 0;

/*
ͨ��WSAStartup������ɶ�WinSock����ĳ�ʼ��
*/
void Socket::start()
{
	if (!nofSockets)
	{
		WSADATA info;
		if (WSAStartup(MAKEWORD(2, 0), &info))
		{
			throw "Windows Sockets Asynchronous Can't StartUp";
		}
	}
	++nofSockets;
}

void Socket::end()
{
	WSACleanup();
}

/*
��SOCKET���з�װ���ṩ���ü���
*/
Socket::Socket() :
	_s(0)
{
	start();
	_s = socket(AF_INET, SOCK_STREAM, 0);

	if (_s == INVALID_SOCKET)
		throw "SOCKET: invalid socket!";

	// BOOL bReuseaddr = TRUE;
	// setsockopt(_s, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReuseaddr, sizeof(BOOL));

	// ���ü���
	refCounter = new int(1);
}

Socket::Socket(SOCKET s) :
	_s(s)
{
	start();

	// BOOL bReuseaddr = TRUE;
	// setsockopt(_s, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReuseaddr, sizeof(BOOL));

	// ���ü���
	refCounter = new int(1);
}

Socket::Socket(const Socket& _socket)
{
	refCounter = _socket.refCounter;
	(*refCounter)++;

	_s = _socket._s;

	nofSockets++;
}

Socket& Socket::operator=(Socket& _socket)
{
	// ���ü�����һ
	(*_socket.refCounter)++;

	refCounter = _socket.refCounter;
	_s = _socket._s;

	nofSockets++;

	return *this;
}

Socket::~Socket()
{
	if (!--(*refCounter))
	{
		close();
		delete refCounter;
	}

	--nofSockets;
	if (!nofSockets)
		end();
}

const SOCKET& Socket::getSocket() const
{
	return _s;
}

void Socket::close()
{
	// �ر��׽���
	closesocket(_s);
}

/*
int recv(SOCKET, buffer, size, flags)������SOCKET�׽��ֶ�ȡ���ݣ�����ȡ�ɹ�ʱ��recv����ʵ�ʶ�ȡ���ֽ�����
������ص���0����ʾ��ȡ��������С��0������˴���
��1�������������ΪEINTR����ʾ����ʱ��������жϣ�
*/
int Socket::read(void* buffer, unsigned int size)
{
	int left_bytes = size;
	int read_bytes = 0;
	char* buffer_ptr = static_cast<char*>(buffer);

	while (left_bytes > 0)
	{
		read_bytes = recv(_s, buffer_ptr, left_bytes, 0);
		if (read_bytes < 0)
		{
			int res = WSAGetLastError();
			if (res == WSAEINTR)
			{
				read_bytes = 0;
			}
			else
			{
				return -1;
			}
		}
		else if (read_bytes == 0)
			break;

		left_bytes -= read_bytes;
		buffer_ptr += read_bytes;
	}

	return (size - read_bytes);
}

int Socket::READ(void* buffer, unsigned int size)
{
	int read_bytes = 0;
	char* buffer_ptr = static_cast<char*>(buffer);

	read_bytes = recv(_s, buffer_ptr, size, 0);
	if (read_bytes < 0)
	{
		int res = WSAGetLastError();
		if (res == WSAEINTR)
		{
			read_bytes = 0;
		}
		else
		{
			return -1;
		}
	}

	return read_bytes;
}

/*
int send(SOCKET, buffer, size, flags)������buffer�е�size�ֽ�����д���ļ�������SOCKET���ɹ�ʱ����д���ֽ�����ʧ�ܷ���-1��
����������У����������׽����ļ�������дʱ��
��1��send����ֵ����0����ʾд���˲��ֻ�����ȫ�����ݡ�
��2��send����ֵС��0����ʱ�����˴���������Ҫ���ݴ������ʹ���
�����������ΪEINTR����ʾд��ʱ��������жϣ�
�����������ΪEPIPE����ʾ�������ӳ��������⣨�Է��ر������ӣ�
*/
int Socket::write(void* buffer, unsigned int size)
{
	int left_bytes = size;
	int send_bytes = 0;
	char* buffer_ptr = static_cast<char*>(buffer);

	while (left_bytes > 0)
	{
		send_bytes = send(_s, buffer_ptr, left_bytes, 0);
		if (send_bytes <= 0)
		{
			int res = WSAGetLastError();
			if (res == WSAEINTR)
			{
				send_bytes = 0;
			}
			else
			{
				return -1;
			}
		}
		left_bytes -= send_bytes;
		buffer_ptr += send_bytes;
	}
	return 0;
}
