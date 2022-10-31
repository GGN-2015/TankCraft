#include <cassert>
#include <iostream>

#include "LoginMessage.h"
#include "RequestMessageTypes.h"
#include "Utils.h"

LoginMessage::LoginMessage()
{
	mLoginStatus = LOGIN_STATUS_UNKNOWN_ERROR;
	mUserID = 0;
}

void LoginMessage::SetStatus(unsigned short nLoginStatus)
{
	mLoginStatus = nLoginStatus;
}

void LoginMessage::SerUserId(int nUserID)
{
	mUserID = nUserID;
}

int LoginMessage::GetStatus() const
{
	return mLoginStatus;
}

std::wstring LoginMessage::GetStatusDescription() const
{
	switch (GetStatus()) {
	case LOGIN_STATUS_SUCCESS:
		return L"��¼�ɹ� (LOGIN_STATUS_SUCCESS)";

	case LOGIN_STATUS_NAME_DUPLICATE:
		return L"��¼���ظ� (LOGIN_STATUS_NAME_DUPLICATED)";

	case LOGIN_STATUS_SERVER_FULL:
		return L"��������Ա (LOGIN_STATUS_SERVER_FULL)";

	case LOGIN_STATUS_IP_BANNED:
		return L"IP�α����� (LOGIN_STATUS_IP_BANNED)";

	case LOGIN_STATUS_UNKNOWN_ERROR:
		return L"δ֪���� (LOGIN_STATUS_UNKNOWN_ERROR)";
	}

	assert(false);
	return L"δ�ҵ�������Ϣ";
}

void LoginMessage::GetRawData(TcpData* tcpData)
{
	char* buf = new char[GetRawDataLength()];

	Utils::DumpUnsignedShortToBuffer(buf, 0, MESSAGE_TYPE_LOGIN_MESSAGE); /* ���� */
	Utils::DumpUnsignedShortToBuffer(buf, 2, GetRawDataLength() - 4);     /* ���ݳ��� = 6 */
	Utils::DumpUnsignedShortToBuffer(buf, 4, mLoginStatus);               /* ״̬ */
	Utils::DumpIntToBuffer          (buf, 6, mUserID);                    /* �û� ID */

	tcpData->SetData(buf, GetRawDataLength());
	delete[] buf;
}

int LoginMessage::GetRawDataLength()
{
	return LOGIN_MESSAGE_LENGTH; /* 2�ֽ���Ϣ���� + 2�ֽڳ�����Ϣ + 2�ֽ�״̬�� + 4�ֽ��û�ID */
	                             /* ��¼ʧ��ʱ��״̬����㣬�û� ID Ϊ�� */
	                             /* ��¼�ɹ�ʱ��״̬��Ϊ�㣬�û� ID ������ */
}

void LoginMessage::Dispatch(ThreadBuffer* tb, GameDatabase* Gdb)
{
	assert(false); /* �˹��ܲ�����ʹ�ã��ͻ���Ҫ����������ʵ�� */
}

void LoginMessage::DebugShow()
{
	std::cerr  <<  "LoginMessage::DebugShow() {\n";
	std::cerr  <<  "    mLoginStatus = " << mLoginStatus << "\n";
	std::cerr  <<  "    mUserID      = " << mUserID << "\n";
	std::wcerr << L"    mDescription = '" << GetStatusDescription() << L"'\n";
	std::cerr  << "}" << std::endl;
}
