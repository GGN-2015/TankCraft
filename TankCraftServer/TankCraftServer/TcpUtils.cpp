#include "TcpUtils.h"

#include <WinSock2.h>

#include <iostream>

#include "GameDatabase.h"
#include "SysUtils.h"
#include "TankCraft_NetManager_Component.h"
#include "TcpClient.h"
#include "TcpData.h"
#include "TcpServer.h"
#include "ThreadBuffer.h"
#include "Utils.h"

void TcpUtils::GetTcpDataFromSocket(void* socketClient, TcpData* tcpData) {
  char* recvBuffer = new char[TCP_BUFFER_MAX];

  int cnt = recv((SOCKET)socketClient, recvBuffer, TCP_BUFFER_MAX, 0);
  tcpData->SetData(recvBuffer, cnt);

  delete[] recvBuffer;  // 一定要特别注意内存泄漏
}

int TcpUtils::SendTcpDataToSocket(const TcpData* tcpData, void* socketClient) {
  // tcpData->DebugShow("[SendToClient] ");
  int ret =
      send((SOCKET)socketClient, tcpData->GetData(), tcpData->GetLength(), 0);
  return ret;
}

void TcpUtils::ClientThreadFunction(
    std::string ip, int port,
    Xn::TankCraft::NetManager_Component* nmComponent) {
  TcpClient tcpClient;

  int ret = tcpClient.Connect(ip.c_str(), port);

  std::cerr << "[Client] Start with " << ip << ":" << port << std::endl;

  if (ret != 0) {
    nmComponent->PushFailedMessage(
        ret); /* 推送一个连接出错消息，告知出错原因 */

    std::cerr << "[Client] End with ErrorLevel =  " << ret << std::endl;
    return; /* 结束线程 */
  } else {
    nmComponent->PushSucessMessage(); /* 推送一个连接成功消息 */

    /* 线程主循环 */
    while (nmComponent->GetConnectStatus() ==
           Xn::TankCraft::NetManager_Component::NET_MANAGER_ONLINE) {
      /* 如果有请求则直接请求 */
      if (nmComponent->HasClientRequest()) {
        std::cerr << "[Client] has request to send." << std::endl;

        TcpDataList tcpDataList;
        Xn::TankCraft::NetMessageBaseDataList nmBaseDataList;

        /* 由于这个操作是原子的，所以要尽可能节省性能 */
        nmComponent->MoveClientRequestToNetMessageBaseDataList(&nmBaseDataList);
        TcpUtils::GetTcpDataListFromNetMessageBaseDataList(&nmBaseDataList,
                                                           &tcpDataList);

        /* 对数据进行打包 */
        // TcpData tcpDataRequest;
        std::shared_ptr<TcpData> tcpDataRequest(TcpData::AllocTcpData(__FILE__, __LINE__, false));

        TcpUtils::CompactTcpDataListToTcpDataRequest(&tcpDataList,
                                                     tcpDataRequest.get());

        /* 发送打包后的请求，获取消息 */
        int sendTime = clock();
        // TcpData tcpDataMessage;
        std::shared_ptr<TcpData> tcpDataMessage(
            TcpData::AllocTcpData(__FILE__, __LINE__));

        tcpClient.Request(tcpDataRequest.get(), tcpDataMessage.get());
        int getTime = clock();

        // TODO: 存储 Ping 值

        /* 检测是否与服务器断开连接 */
        if (tcpDataMessage->IsEnd()) {
          nmComponent->PushFailedMessage(TCP_CLIENT_DISCONNECT_FROM_SERVER);
          std::cerr << "[Client] Can not connect to server." << std::endl;
        }

        /* 释放先前的 Request 空间 */
        for (auto pTcpData : tcpDataList) {
          delete pTcpData;
        }
        tcpDataList.clear();

        /* 对收到的消息进行解包 */
        if (!tcpDataMessage->IsEnd()) {
          TcpUtils::UnpackTcpDataMessageToTcpDataList(tcpDataMessage.get(),
                                                      &tcpDataList);

          /* 把拆分后的数据送入消息队列 */
          for (auto pTcpData : tcpDataList) {
            nmComponent->PushServerMessageTcpData(pTcpData);
          }

          for (auto pTcpData : tcpDataList) { /* 释放 Message 空间 */
            delete pTcpData;
          }
          tcpDataList.clear();
        } else
          break;
      } else {
        /* 如果没有请求也要发送一个 ping 保证自己能够刷新地图 */
        unsigned short xVal = Utils::GetRandLongLong() % 65536;
        nmComponent->PushPingMessage(xVal);
      }

      SysUtils::Sleep(CLIENT_THREAD_SLEEP_TIME);
    }

    /* 断开连接 */
    tcpClient.CloseSocket();
    std::cerr << "[Client End]" << std::endl;
  }
}

void TcpUtils::CompactTcpDataListToTcpDataRequest(
    const TcpDataList* tcpDataList, TcpData* tcpDataRequest) {
  int totalLength = 4; /* 头部信息两字节，校验信息两字节 */
  int totalCnt = (int)tcpDataList->size();

  /* 计算二进制信息总长度 */
  for (auto pTcpData : *tcpDataList) {
    totalLength += pTcpData->GetLength();
  }

  /* 申请缓冲区 */
  char* buf = new char[totalLength];
  int pos = 0;
  Utils::DumpUnsignedShortToBuffer(buf, pos, (unsigned short)totalCnt);
  pos += 2;

  /* 将数据填写进入消息 */
  for (auto pTcpData : *tcpDataList) {
    Utils::DumpTcpDataToBuffer(buf, pos, pTcpData);
    pos += pTcpData->GetLength();
  }
  assert(pos == totalLength - 2); /* 检查是否恰好还剩两个位置 */

  /* 计算校验和 */
  char evenAns = 0, oddAns = 0;
  Utils::GetSanityInteger(buf, pos, &evenAns, &oddAns);
  buf[pos + 0] = evenAns;
  buf[pos + 1] = oddAns;

  tcpDataRequest->SetData(buf, totalLength);
  delete[] buf;
}

void TcpUtils::UnpackTcpDataMessageToTcpDataList(const TcpData* pTcpDataMessage,
                                                 TcpDataList* pTcpDataList) {
  assert(pTcpDataMessage->GetSanity());
  int messageTotalLength = pTcpDataMessage->GetLength() - 2;

  /* 取出消息总数 */
  int pos = 0;
  unsigned short messageCnt =
      Utils::GetUnsignedShort(pTcpDataMessage->GetData(), pos);
  int cnt = 0;
  pos += 2;

  while (pos < messageTotalLength) {
    /* 从 pos 开始长度为 dataLength 的一段是一个消息 */
    unsigned short dataLength =
        Utils::GetUnsignedShort(pTcpDataMessage->GetData(), pos + 2) + 4;

    pTcpDataMessage->DebugShow("[TcpData] ");
    assert(pos + dataLength <= messageTotalLength);

    TcpData* tcpDataNow = TcpData::AllocTcpData(__FILE__, __LINE__);
    tcpDataNow->SetData(pTcpDataMessage->GetData() + pos, dataLength);
    pTcpDataList->push_back(tcpDataNow); /* 主函数负责释放 */

    cnt += 1;
    pos += dataLength;
  }

  assert(pos == messageTotalLength);
  assert(cnt == messageCnt);
}

void TcpUtils::GetTcpDataListFromNetMessageBaseDataList(
    Xn::TankCraft::NetMessageBaseDataList* nmBaseDataList,
    TcpDataList* tcpDataList) {
  tcpDataList->clear();

  for (int i = 0; i < (int)nmBaseDataList->size(); i += 1) {
    TcpData* tcpDataNow = TcpData::AllocTcpData(__FILE__, __LINE__);
    (*nmBaseDataList)[i]->MoveDataToTcpData(tcpDataNow); /* 移动数据 */

    /* TODO 要记得释放 */
    tcpDataList->push_back(tcpDataNow);
  }
}
