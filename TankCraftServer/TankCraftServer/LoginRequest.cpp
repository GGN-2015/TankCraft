#include <iostream>

#include "GameDatabase.h"
#include "LoginRequest.h"
#include "LoginMessage.h"
#include "RequestMessageTypes.h"
#include "TcpData.h"
#include "ThreadBuffer.h"
#include "Utils.h"

int LoginRequest::GetRawDataLength()
{
    return 4 + 2 * (int)mUserName.length();
}

void LoginRequest::Dispatch(ThreadBuffer* tb, GameDatabase* Gdb)
{
    /* 已经在游戏中的玩家，再次登录的请求会被忽略 */
    if (!tb->InGame()) {
        Gdb->lock(); /* 加锁 */
        
        /* 向客户端反馈的消息 */  
        LoginMessage* pLoginMessage = new LoginMessage;

        if (!Gdb->CheckUserNameExist(mUserName)) { /* 用户名没出现过 */

            if (Gdb->GetUserCount() < GAME_DATABASE_USER_MAX) { /* 人数没有达到上限 */

                /* 当前版本忽略了 BAN_IP 和其他错误 */

                int nUserID = Gdb->AllocNxtUserId();

                pLoginMessage->SetStatus(LOGIN_STATUS_SUCCESS); // 0
                pLoginMessage->SerUserId(nUserID);

                tb->SetUserID(nUserID);
                Gdb->AddUser(nUserID, mUserName);
            }
            else { /* 人数达到上限，登陆失败 */
                pLoginMessage->SetStatus(LOGIN_STATUS_SERVER_FULL); // 1
            }
        }
        else { /* 用户名出现过，登陆失败 */
            pLoginMessage->SetStatus(LOGIN_STATUS_NAME_DUPLICATE); // 2
        }

        /* 反馈消息加入反馈队列 */
        tb->DumpMessage(std::shared_ptr<IMessage>(pLoginMessage));

        Gdb->unlock(); /* 解锁 */
    }
}

void LoginRequest::SetUserName(const std::wstring& nUserName)
{
    mUserName = nUserName;
}

std::wstring LoginRequest::GetUserName() const
{
    return mUserName;
}

void LoginRequest::GetRawData(TcpData* tcpData)
{
    int dataByteCnt = GetRawDataLength() - 4;

    /* 把输入送入缓冲区 */
    char* buf = new char[GetRawDataLength()];
    Utils::DumpUnsignedShortToBuffer(buf, 0, REQUEST_TYPE_LOGIN_REQUEST);
    Utils::DumpUnsignedShortToBuffer(buf, 2, dataByteCnt);
    Utils::DumpRawStringToBuffer    (buf, 4, (const char*)mUserName.c_str(), dataByteCnt);

    tcpData->SetData(buf, GetRawDataLength());
    delete[] buf;
}

void LoginRequest::DebugShow()
{
    /* 输出用户名信息 */
    std::cerr << "LoginRequest::DebugShow() {mUserName = '";
    std::wcerr << mUserName;
    std::cerr << "'}" << std::endl;
}
