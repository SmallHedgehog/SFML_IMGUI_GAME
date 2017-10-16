#include "msgHandleThread.h"


msgHandleThread::msgHandleThread(MsgQueue* _msg, MsgHandle* _msgHandle):
	msgQueue(_msg), msgHandle(_msgHandle)
{
	// do something
}

void msgHandleThread::run()
{
	MESSAGE message;
	while (true)
	{
		(*msgQueue) >> message;
		// ����õ�����Ϣ
		/*
		if (message.data.dataHeader.msgType == MessageType::TYPE_SIGNOUT)
		{
			// ����ǳ���Ϣ
			std::cout << "msgHandle->handleCliClose" << std::endl;
			msgHandle->handleCliClose(message.cSocket, "");
		}
		else
		{
			if (msgHandle)
				msgHandle->handleCliConn_MsgRecv(message);
		}
		*/
		if (msgHandle)
			msgHandle->handleCliConn_MsgRecv(message);
	}
}
