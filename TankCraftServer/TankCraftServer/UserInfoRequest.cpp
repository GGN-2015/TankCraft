#include <cassert>
#include <iostream>

#include "GameDatabase.h"
#include "TcpData.h"
#include "ThreadBuffer.h"
#include "UserInfoMessage.h"
#include "UserInfoRequest.h"

int UserInfoRequest::GetRawDataLength()
{
    return 4;
}

void UserInfoRequest::Dispatch(ThreadBuffer* tb, GameDatabase* Gdb)
{
    // TODO: ����Ч�ʱȽϵͣ���Ҫ�Ż��ٽ������㷨 
    Gdb->lock();

    // TcpData pTcpData;

    std::shared_ptr<TcpData> pTcpData(
        TcpData::AllocTcpData(__FILE__, __LINE__, false));

    Gdb->GetTcpDataForUserInfoMessage(pTcpData.get()); /* ��ȡȫ���û����� */
    int maxUserId = Gdb->GetMaxUserId();

    Gdb->unlock();

    UserInfoMessage* pUserInfoMessage = new UserInfoMessage(pTcpData.get());
    tb->DumpMessage(pUserInfoMessage);
    tb->SetMaxSentUserId(maxUserId);
}

void UserInfoRequest::GetRawData(TcpData* tcpData)
{
    assert(false);
}

void UserInfoRequest::DebugShow()
{
    std::cerr << "[UserInfoRequest]" << std::endl;
}
