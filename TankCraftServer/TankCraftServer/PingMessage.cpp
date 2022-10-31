#include <iostream>

#include "PingMessage.h"
#include "ThreadBuffer.h"
#include "Utils.h"

#define PING_MESSAGE_DEBUG

PingMessage::PingMessage()
{
    mX = 65535; // +1 ֮����� 0
}

int PingMessage::GetRawDataLength()
{
    return PING_MESSAHE_LENGTH;
}

/* �յ� ping message ����ͻ��˷��� ping message */
void PingMessage::Dispatch(ThreadBuffer* tb, GameDatabase* Gdb)
{
    /* ����Ӧ����Ϣ */
    PingMessage* pPingMsgRev = new PingMessage;
    pPingMsgRev->SetX(((unsigned int)GetX() + 1) % 65536);

    /* ��Ӧ����Ϣ�������� */
    tb->DumpMessage(pPingMsgRev);

    /* ע�⣺tb ����� DumpMessage �е� Message �����ͷ� */
}

void PingMessage::SetX(unsigned short nX)
{
    mX = nX;
#ifdef PING_MESSAGE_DEBUG
    std::cerr << "PingMessage::SetX(" << mX << ")" << std::endl;
#endif
}

unsigned short PingMessage::GetX() const
{
    return mX;
}

void PingMessage::GetRawData(TcpData* tcpData)
{
    char buf[PING_MESSAHE_LENGTH] = {};
    Utils::DumpUnsignedShortToBuffer(buf, 0, 0);
    Utils::DumpUnsignedShortToBuffer(buf, 2, mX);

    /* �����ݴ洢�� TcpData �� */
    tcpData->SetData(buf, PING_MESSAHE_LENGTH);
}
