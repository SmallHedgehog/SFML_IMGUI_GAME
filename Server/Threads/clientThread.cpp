#include "clientThread.h"


clientThread::clientThread(Socket* _clientSocket, MsgQueue* _msg):
	clientSocket(_clientSocket), msgQueue(_msg)
{
	// do something
	isClosed = false;
}

void clientThread::run()
{
	char buffer[256] = { 0 };

	while (true)
	{
		if (!clientSocket || isClosed)
			break;

		int recvSize = clientSocket->READ(buffer, sizeof(buffer));
		if (recvSize > 0)
		{
			// ��Ҫ����TCPճ�������
			tcpDataSplit(buffer, recvSize);
		}
		else
		{
			if (!isClosed)
			{
				std::cout << "�ͻ����Ѿ��ر�" << std::endl;
				// �ͻ������ӹر�
				DATA_PACKAGE dataMsg;
				dataMsg.dataHeader.msgType = MessageType::TYPE_SIGNOUT;
				strcpy(dataMsg.data, "NONE");
				MESSAGE msg(clientSocket, dataMsg);
				if (msgQueue)
				{
					(*msgQueue) << msg;
				}
				clientSocket = nullptr;
				isClosed = true;
			}
			break;
		}
	}
}

void clientThread::handleRecvData(DATA_HEADER* pHeader)
{
	// ��Ϣ����
	// MessageType msgType = pHeader->msgType;
	// ���ݰ�
	DATA_PACKAGE* data = (DATA_PACKAGE*)pHeader;
	/*
	std::cout << data->dataHeader.msgType << std::endl;
	std::cout << data->dataHeader.dataSize << std::endl;
	std::cout << data->data << std::endl;
	std::cout << clientSocket->addr << std::endl;
	*/

	DATA_PACKAGE dataMsg;
	memcpy(&dataMsg, data, sizeof(DATA_PACKAGE));
	if (dataMsg.dataHeader.msgType == MessageType::TYPE_SIGNOUT)
	{
		// �ǳ���Ϣ
		isClosed = true;
	}

	// std::cout << dataMsg.dataHeader.dataSize << std::endl;
	// std::cout << dataMsg.data << std::endl;

	// ������Ϣ�����е���Ϣ
	MESSAGE msg(clientSocket, dataMsg);
	if (msgQueue)
	{
		(*msgQueue) << msg;
	}
}

void clientThread::tcpDataSplit(const char* recvData, int recvSize)
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
