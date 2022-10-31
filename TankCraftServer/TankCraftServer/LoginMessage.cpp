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
		return L"登录成功 (LOGIN_STATUS_SUCCESS)";

	case LOGIN_STATUS_NAME_DUPLICATE:
		return L"登录名重复 (LOGIN_STATUS_NAME_DUPLICATED)";

	case LOGIN_STATUS_SERVER_FULL:
		return L"服务器满员 (LOGIN_STATUS_SERVER_FULL)";

	case LOGIN_STATUS_IP_BANNED:
		return L"IP段被禁用 (LOGIN_STATUS_IP_BANNED)";

	case LOGIN_STATUS_UNKNOWN_ERROR:
		return L"未知错误 (LOGIN_STATUS_UNKNOWN_ERROR)";
	}

	assert(false);
	return L"未找到错误信息";
}

void LoginMessage::GetRawData(TcpData* tcpData)
{
	char* buf = new char[GetRawDataLength()];

	Utils::DumpUnsignedShortToBuffer(buf, 0, MESSAGE_TYPE_LOGIN_MESSAGE); /* 类型 */
	Utils::DumpUnsignedShortToBuffer(buf, 2, GetRawDataLength() - 4);     /* 数据长度 = 6 */
	Utils::DumpUnsignedShortToBuffer(buf, 4, mLoginStatus);               /* 状态 */
	Utils::DumpIntToBuffer          (buf, 6, mUserID);                    /* 用户 ID */

	tcpData->SetData(buf, GetRawDataLength());
	delete[] buf;
}

int LoginMessage::GetRawDataLength()
{
	return LOGIN_MESSAGE_LENGTH; /* 2字节消息类型 + 2字节长度信息 + 2字节状态码 + 4字节用户ID */
	                             /* 登录失败时，状态码非零，用户 ID 为零 */
	                             /* 登录成功时，状态码为零，用户 ID 大于零 */
}

void LoginMessage::Dispatch(ThreadBuffer* tb, GameDatabase* Gdb)
{
	assert(false); /* 此功能不允许使用，客户端要采用其他的实现 */
}

void LoginMessage::DebugShow()
{
	std::cerr  <<  "LoginMessage::DebugShow() {\n";
	std::cerr  <<  "    mLoginStatus = " << mLoginStatus << "\n";
	std::cerr  <<  "    mUserID      = " << mUserID << "\n";
	std::wcerr << L"    mDescription = '" << GetStatusDescription() << L"'\n";
	std::cerr  << "}" << std::endl;
}
