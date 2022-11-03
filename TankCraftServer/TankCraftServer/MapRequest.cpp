#include <cassert>
#include <iostream>

#include "MapRequest.h"
#include "ThreadBuffer.h"

int MapRequest::GetRawDataLength()
{
    return 4; /* 类型 = 2, 长度 = 0 */
}

void MapRequest::Dispatch(ThreadBuffer* tb, GameDatabase* Gdb)
{
    tb->DumpGraphTcpDataIntoMessageList(Gdb);
    tb->SetGraphTimer(); /* 记录当前时刻以为更新地图做准备 */
}

void MapRequest::GetRawData(TcpData* tcpData)
{
    assert(false); /* 不允许获得 rawData */
}

void MapRequest::DebugShow()
{
    std::cerr << "[MapRequest]" << std::endl;
}
