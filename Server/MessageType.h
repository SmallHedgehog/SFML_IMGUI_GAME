#pragma once

// ������Ϣ����
enum MessageType
{
	TYPE_SIGNIN,				// ��¼��Ϣ����
	TYPE_SIGNUP,				// ע����Ϣ����
	TYPE_SIGNOUT,				// �ǳ���Ϣ����
	TYPE_USER,					// �û���Ϣ����
	TYPE_MATCH,					// ƥ����Ϣ����
	TYPE_FIGHT,					// ��ս��Ϣ����
	TYPE_NONE
};


// ������������

#define MAX_DATA_SIZE	128		// max data size

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
