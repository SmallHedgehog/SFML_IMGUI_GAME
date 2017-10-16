#pragma once

#include <iostream>
#include "virtualThread.h"
#include "../Network/Socket.h"
#include "../MessageType.h"
#include "../MsgQueue.h"


/*
�������Կͻ��˵����ӣ�clientThread����ÿ�οͻ�������ʱ���ɿͻ����̴߳���
*/

class clientThread : public virtualThread
{
public:
	clientThread(Socket* _clientSocket, MsgQueue* _msg);
	// TCPճ������
	void tcpDataSplit(const char* recvData, int recvSize);
	// ������յ�������
	void handleRecvData(DATA_HEADER* pHeader);
	virtual void run();

	// ��ȡ�ͻ���Socket
	Socket* get_cSocket() { return clientSocket; }

private:
	Socket* clientSocket;
	// ��Ϣ����
	MsgQueue* msgQueue;

	// �����Ƿ�ر�
	bool isClosed;
};

