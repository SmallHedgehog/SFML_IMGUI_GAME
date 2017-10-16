#pragma once

#include "virtualThread.h"
#include "../MsgHandle/MsgHandle.h"
#include "../MsgQueue.h"


// ����Ϣ�����л�ȡһ����Ϣ

class msgHandleThread : public virtualThread
{
public:
	msgHandleThread(MsgQueue* _msg, MsgHandle* _msgHandle);
	// ~msgHandleThread();

	virtual void run();

private:
	// ��Ϣ����
	MsgQueue* msgQueue;

	// ��Ϣ����ģ��
	MsgHandle* msgHandle;
};

