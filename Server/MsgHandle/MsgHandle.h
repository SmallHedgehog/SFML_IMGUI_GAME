#pragma once

/*
�̳�AbstractHandle�����࣬ʵ��
��1�����տͻ������ӡ��յ��ͻ�����Ϣ��
��2���ͻ������ӹرյ���Ϣ��
�ȳ���������
*/

#include "AbstractHandle.h"
#include "../MessageType.h"
#include "../Threads/virtualThread.h"
#include "../Threads/clientThread.h"
#include "DataAccessObject/DBManager.h"
#include "Logic/Login.h"
#include "Logic/Fight.h"
#include "Logic/Match.h"
#include "Logic/User.h"
#include "Logic/AbstractLogic.h"


class MsgHandle : public AbstractHandle
{
public:
	MsgHandle();
	virtual ~MsgHandle();

public:
	// �ͻ������ӡ��յ��ͻ�����Ϣ
	void handleCliConn_MsgRecv(MESSAGE& _msg);

	// �ͻ������ӹر�
	void handleCliClose(Socket* _clientSocket, const char* _error);

	// �ͻ������ӶϿ�ʱ���ͷſͻ���sockets���ͻ����߳�
	void releaseResource(Socket* _clientSocket);

public:
	enum logicType
	{
		SIGN,		// ��¼��ע��
		USER,
		FIGHT,
		MATCH,
		SUM_LOGICTYPE
	};

private:
	// �߼�����
	AbstractLogic* logicHandles[SUM_LOGICTYPE];

	// ���пͻ���sockets
	// std::list<Socket*>& cSockets;

	// ���д���ͻ��˵��߳�
	// std::list<virtualThread*>& cThreads;

	// ���ݿ����Ӷ���
	DBManager* db;
};
