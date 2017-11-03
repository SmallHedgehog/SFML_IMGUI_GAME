#include "MsgRecvThread.h"

MsgRecvThread::MsgRecvThread(Client* _cli, MsgQueue* _msg):
	msg(_msg),
	cli(_cli)
{}

void MsgRecvThread::run()
{
	char buffer[256] = { 0 };

	while (true)
	{
		if (!cli)
			break;

		int recvSize = cli->READ(buffer, sizeof(buffer));
		if (recvSize > 0)
		{
			// TCPճ��������յ�������
			tcpDataSplit(buffer, recvSize);
		}
		else
		{
			std::cout << "�����쳣..." << std::endl;
			DATA_PACKAGE dPackage;
			// ConsData("CONNECT ERROR!", MessageType::TYPE_SIGNOUT, dPackage);
			FilldPackage(MessageType::TYPE_SIGNOUT, SMessageTypeFlags_IsTheUserSingout, "CONNECT ERROR!", dPackage);
			if (msg)
				(*msg) << dPackage;
			break;
		}
	}
}

void MsgRecvThread::tcpDataSplit(const char* recvData, int recvSize)
{
	static char lastRecvData[MAX_DATA_SIZE * 4];
	static unsigned int currentSize = 0;
	static bool isFirst = true;

	if (isFirst)
	{
		memset(lastRecvData, 0, sizeof(lastRecvData));
		isFirst = false;
	}

	// ���ν��յ�������ƴ�ӵ��ϴ�����
	memcpy((char*)(lastRecvData + currentSize), recvData, recvSize);
	currentSize += recvSize;

	// ǿ��ת����DATA_HEADER���͵�ָ��
	DATA_HEADER* hPtr = (DATA_HEADER*)lastRecvData;
	while (currentSize > sizeof(DATA_HEADER) &&
		currentSize >= (hPtr->dataSize + sizeof(DATA_HEADER)))
	{
		// ������յ�������
		handleRecvData(hPtr);
		// ���ν��ܵ��Ķ����С
		int recvObjectSize = sizeof(DATA_HEADER) + hPtr->dataSize;
		currentSize -= recvObjectSize;
		// �ƶ���һ������
		hPtr = (DATA_HEADER*)((char*)hPtr + recvObjectSize);
	}

	// ʣ��Ķ���δ�����һ������Ĵ�С����Ҫ��������
	if (lastRecvData != (char*)hPtr)
	{
		memmove(lastRecvData, (char*)hPtr, currentSize);
		memset((char*)(lastRecvData + currentSize), 0, sizeof(lastRecvData) - currentSize);
	}
}

void MsgRecvThread::handleRecvData(DATA_HEADER* pHeader)
{
	// ���ݰ�
	DATA_PACKAGE* data = (DATA_PACKAGE*)pHeader;
	DATA_PACKAGE dataMsg;
	memcpy(&dataMsg, data, sizeof(DATA_PACKAGE));

	if (msg)
		(*msg) << dataMsg;
}
