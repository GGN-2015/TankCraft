#include <cassert>
#include <iostream>

#include "MapRequest.h"
#include "ThreadBuffer.h"

int MapRequest::GetRawDataLength()
{
    return 4; /* ���� = 2, ���� = 0 */
}

void MapRequest::Dispatch(ThreadBuffer* tb, GameDatabase* Gdb)
{
    tb->DumpGraphTcpDataIntoMessageList(Gdb);
    tb->SetGraphTimer(); /* ��¼��ǰʱ����Ϊ���µ�ͼ��׼�� */
}

void MapRequest::GetRawData(TcpData* tcpData)
{
    assert(false); /* �������� rawData */
}

void MapRequest::DebugShow()
{
    std::cerr << "[MapRequest]" << std::endl;
}
