#pragma once

#include "BaseThread.h"
#include "../Network/Client.h"
#include "../MsgQueue/MsgQueue.h"

#include <iostream>

/*
��Ϣ�����̣߳������յ����߳�������Ϣ����
*/


class MsgRecvThread : public BaseThread
{
public:
	MsgRecvThread(Client* _cli, MsgQueue* _msg);
	virtual void run();

private:
	// TCPճ������
	void tcpDataSplit(const char* recvData, int recvSize);
	// ������յ�������
	void handleRecvData(DATA_HEADER* pHeader);

private:
	MsgQueue* msg;

	Client* cli;
};
