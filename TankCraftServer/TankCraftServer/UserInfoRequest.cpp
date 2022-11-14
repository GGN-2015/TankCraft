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
    // TODO: 这里效率比较低，需要优化临界区的算法 
    Gdb->lock();

    // TcpData pTcpData;

    std::shared_ptr<TcpData> pTcpData(
        TcpData::AllocTcpData(__FILE__, __LINE__, false));

    Gdb->GetTcpDataForUserInfoMessage(pTcpData.get()); /* 获取全部用户数据 */
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
