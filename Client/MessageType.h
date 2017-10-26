#pragma once

#include <string>

#define DEBUG 1

// ������Ϣ����
enum MessageType
{
	TYPE_SIGNIN,				// ��¼��Ϣ����
	TYPE_SIGNUP,				// ע����Ϣ����
	TYPE_SIGNOUT,				// �ǳ���Ϣ����
	TYPE_USER,					// �û���Ϣ����
	TYPE_MATCH,					// ƥ����Ϣ����
	TYPE_FIGHT,					// ��ս��Ϣ����
	TYPE_CHAT,					// Chat message
	TYPE_NONE
};


// ������������

#define MAX_DATA_SIZE	1024	// max data size

/* ���ݰ�ͷ���� */
typedef struct data_header
{
	MessageType msgType;		// ��Ϣ����
	unsigned int dataSize;		// ���ݰ�����ʵ���ݵĳ���

	data_header& operator = (const data_header& _data)
	{
		if (this != &_data)
		{
			msgType = _data.msgType;
			dataSize = _data.dataSize;
		}
		return *this;
	}
}DATA_HEADER;

/* ���ݰ����Ͷ��� */
typedef struct data_package
{
	DATA_HEADER dataHeader;		// ���ݰ�ͷ
	char data[MAX_DATA_SIZE];	// ��ʵ����

	data_package& operator = (const data_package& _data)
	{
		if (this != &_data)
		{
			dataHeader = _data.dataHeader;
			memcpy(data, _data.data, sizeof(data));
		}
		return *this;
	}
}DATA_PACKAGE;

/* ��Ϣ״̬���� */
typedef struct msg_state
{
	// username
	std::string uName;

	// real-time message
	MessageType type;
	char info[128];

	msg_state()
	{
		type = MessageType::TYPE_SIGNIN;
		// info = nullptr;
	}
}MSG_STATE;

static void ConsData(const char* _msg, MessageType type, DATA_PACKAGE& dPackage)
{
	dPackage.dataHeader.msgType = type;
	dPackage.dataHeader.dataSize = sizeof(dPackage) - sizeof(DATA_HEADER);

	strcpy(dPackage.data, _msg);
}
