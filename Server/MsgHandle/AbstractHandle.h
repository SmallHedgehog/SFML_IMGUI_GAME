#pragma once

/*
���������Ϣ�����������������ʵ�ֳ��󷽷�������ֻ����
��1�����տͻ������ӡ��յ��ͻ�����Ϣ��
��2���ͻ������ӹرյ���Ϣ��
�ȳ���������
*/

#include "../MsgQueue.h"


class AbstractHandle
{
public:
	// �ͻ������ӡ��յ��ͻ�����Ϣ
	virtual void handleCliConn_MsgRecv(MESSAGE& _msg) = 0;

	// �ͻ������ӹر�
	virtual void handleCliClose(Socket* _clientSocket, const char* _error) = 0;
};
