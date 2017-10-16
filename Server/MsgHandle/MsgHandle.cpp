#include "MsgHandle.h"


MsgHandle::MsgHandle()
{
	// ��¼��ע��
	logicHandles[SIGN] = new Login();

	logicHandles[USER] = new User();
	logicHandles[FIGHT] = new Fight();
	logicHandles[MATCH] = new Match();

	// ��ʼ�����ݿ����Ӷ���
	db = DBManager::getInstance();
	db->init("120.25.163.214", "root", "123456");
}

void MsgHandle::handleCliConn_MsgRecv(MESSAGE& _msg)
{
	// ��Ϣ����
	MessageType msgType = _msg.data.dataHeader.msgType;

	switch (msgType)
	{
	case MessageType::TYPE_SIGNIN:		// ��¼��Ϣ����
		logicHandles[SIGN]->handleMessage(_msg.cSocket, _msg.data.data, _msg.data.dataHeader.dataSize,
			MessageType::TYPE_SIGNIN);
		break;
	case MessageType::TYPE_SIGNUP:		// ע����Ϣ����
		logicHandles[SIGN]->handleMessage(_msg.cSocket, _msg.data.data, _msg.data.dataHeader.dataSize,
			MessageType::TYPE_SIGNUP);
		break;
	case MessageType::TYPE_SIGNOUT:		// �ǳ���Ϣ����
		logicHandles[SIGN]->handleMessage(_msg.cSocket, _msg.data.data, _msg.data.dataHeader.dataSize,
			MessageType::TYPE_SIGNOUT);
		handleCliClose(_msg.cSocket, "");
		break;
	case MessageType::TYPE_USER:		// �û���Ϣ����
		logicHandles[USER]->handleMessage(_msg.cSocket, _msg.data.data, _msg.data.dataHeader.dataSize);
		break;
	case MessageType::TYPE_FIGHT:		// ��ս��Ϣ����
		logicHandles[FIGHT]->handleMessage(_msg.cSocket, _msg.data.data, _msg.data.dataHeader.dataSize);
		break;
	case MessageType::TYPE_MATCH:		// ƥ����Ϣ����
		logicHandles[MATCH]->handleMessage(_msg.cSocket, _msg.data.data, _msg.data.dataHeader.dataSize);
		break;
	default:
		break;
	}
}

void MsgHandle::handleCliClose(Socket* _clientSocket, const char* _error)
{
	// �ͻ������ӶϿ�ʱ���ͷſͻ���sockets���ͻ����߳�
	releaseResource(_clientSocket);
}

void MsgHandle::releaseResource(Socket* _clientSocket)
{
/*����Ҫ��ɾ������ͻ��˵��̣߳�Ȼ����ɾ�ͻ���Socket*/
	// ɾ���ͻ��˹ر�ʱ����ͻ��˵��߳�
	for (std::list<virtualThread*>::iterator it = Cache::cThreads.begin(); it != Cache::cThreads.end(); )
	{
		clientThread* cThread = dynamic_cast<clientThread*>((*it));
		if (cThread)
		{
			Socket* cSocket = cThread->get_cSocket();
			if (&(*cSocket) == &(*(_clientSocket)))
			{
				cThread->join();
				delete (*it);
				it = Cache::cThreads.erase(it);
			}
			else
				++it;
		}
		else
			++it;
	}
	// ɾ���ͻ��˹ر�ʱ�Ŀͻ���Socket
	for (std::list<Socket*>::iterator it = Cache::cSockets.begin(); it != Cache::cSockets.end(); )
	{
		if (&(*(*it)) == &(*(_clientSocket)))
		{
			delete (*it);
			it = Cache::cSockets.erase(it);
		}
		else
			++it;
	}
}

MsgHandle::~MsgHandle()
{
	for (int i = 0; i < (int)SUM_LOGICTYPE; ++i)
	{
		if (logicHandles[i])
			delete logicHandles[i];
	}

	if (db)
		delete db;
}
