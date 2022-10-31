#include <iostream>

#include "PingMessage.h"
#include "RequestMessageTypes.h"
#include "ThreadBuffer.h"
#include "Utils.h"

//#define PING_MESSAGE_DEBUG

PingMessage::PingMessage()
{
    mX = 65535; // +1 之后等于 0
}

int PingMessage::GetRawDataLength()
{
    return PING_MESSAHE_LENGTH;
}

/* 收到 ping message 后，向客户端发送 ping message */
void PingMessage::Dispatch(ThreadBuffer* tb, GameDatabase* Gdb)
{
    /* 生成应答消息 */
    PingMessage* pPingMsgRev = new PingMessage;
    pPingMsgRev->SetX(((unsigned int)GetX() + 1) % 65536);

    /* 将应答消息缓存起来 */
    tb->DumpMessage(pPingMsgRev);

    /* 注意：tb 负责对 DumpMessage 中的 Message 进行释放 */
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
    Utils::DumpUnsignedShortToBuffer(buf, 0, REQUEST_TYPE_PING); /* 类型信息 = 0 */
    Utils::DumpUnsignedShortToBuffer(buf, 2, 2);                 /* 数据部分长度 2 字节 */
    Utils::DumpUnsignedShortToBuffer(buf, 2, mX);                /* 具体内容是 mX */

    /* 将数据存储到 TcpData 中 */
    tcpData->SetData(buf, PING_MESSAHE_LENGTH);
}

void PingMessage::DebugShow()
{
    std::cerr << "PingMessage::DebugShow() {mX = " << mX << "}" << std::endl;
}
