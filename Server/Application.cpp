#include "Application.h"


Application::Application(unsigned short port, unsigned int connections) :
	server(nullptr), msg(nullptr)
{
	msg = new MsgQueue();
	if (!msg)
	{
		throw "APPLICATION: create MsgQueue error!";
	}

	msgHandle = new MsgHandle();
	if (!msgHandle)
	{
		throw "APPLICATION: create MsgHandle error!";
	}

	server = new Server(port, connections);
	if (!server)
	{
		throw "APPLICATION: create server error!";
	}
}

void Application::run()
{
	// ������Ϣ�����̣߳����ڼ�����Ϣ�����е���Ϣ
	msgHandleThread* msgThread = new msgHandleThread(msg, msgHandle);
	msgThread->start();
	Cache::cThreads.push_back(msgThread);
	while (true)
	{
		// �����������Կͻ��˵�����
		std::cout << "�������Կͻ��˵�����" << std::endl;
		Socket* s = server->Accept();
		if (s)
		{
			Cache::cSockets.push_back(s);
			// �����ͻ����̣߳��������Կͻ��˵�����
			clientThread* cThread = new clientThread(s, msg);
			cThread->start();
			Cache::cThreads.push_back(cThread);
		}
	}

	for (std::list<virtualThread*>::iterator it = Cache::cThreads.begin(); it != Cache::cThreads.end(); ++it)
	{
		if (*it)
			(*it)->join();
	}
}

Application::~Application()
{
	if (server)
		delete server;

	if (msgHandle)
		delete msgHandle;

	if (msg)
		delete msg;
	// �ͻ��˴����߳�
	for (std::list<virtualThread*>::iterator it = Cache::cThreads.begin(); it != Cache::cThreads.end(); ++it)
	{
		if (*it)
			delete (*it);
	}
	Cache::cThreads.clear();

	// �ͻ�������Socket
	for (std::list<Socket*>::iterator it = Cache::cSockets.begin(); it != Cache::cSockets.end(); ++it)
	{
		if (*it)
			delete (*it);
	}
	Cache::cSockets.clear();
}
