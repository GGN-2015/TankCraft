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
    /* �Ѿ�����Ϸ�е���ң��ٴε�¼������ᱻ���� */
    if (!tb->InGame()) {
        Gdb->lock(); /* ���� */
        
        /* ��ͻ��˷�������Ϣ */  
        LoginMessage* pLoginMessage = new LoginMessage;

        if (!Gdb->CheckUserNameExist(mUserName)) { /* �û���û���ֹ� */

            if (Gdb->GetUserCount() < GAME_DATABASE_USER_MAX) { /* ����û�дﵽ���� */

                /* ��ǰ�汾������ BAN_IP ���������� */

                int nUserID = Gdb->AllocNxtUserId();

                pLoginMessage->SetStatus(LOGIN_STATUS_SUCCESS); // 0
                pLoginMessage->SerUserId(nUserID);

                tb->SetUserID(nUserID);
                Gdb->AddUser(nUserID, mUserName);
            }
            else { /* �����ﵽ���ޣ���½ʧ�� */
                pLoginMessage->SetStatus(LOGIN_STATUS_SERVER_FULL); // 1
            }
        }
        else { /* �û������ֹ�����½ʧ�� */
            pLoginMessage->SetStatus(LOGIN_STATUS_NAME_DUPLICATE); // 2
        }

        /* ������Ϣ���뷴������ */
        tb->DumpMessage(std::shared_ptr<IMessage>(pLoginMessage));

        Gdb->unlock(); /* ���� */
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

    /* ���������뻺���� */
    char* buf = new char[GetRawDataLength()];
    Utils::DumpUnsignedShortToBuffer(buf, 0, REQUEST_TYPE_LOGIN_REQUEST);
    Utils::DumpUnsignedShortToBuffer(buf, 2, dataByteCnt);
    Utils::DumpRawStringToBuffer    (buf, 4, (const char*)mUserName.c_str(), dataByteCnt);

    tcpData->SetData(buf, GetRawDataLength());
    delete[] buf;
}

void LoginRequest::DebugShow()
{
    /* ����û�����Ϣ */
    std::cerr << "LoginRequest::DebugShow() {mUserName = '";
    std::wcerr << mUserName;
    std::cerr << "'}" << std::endl;
}
