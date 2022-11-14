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

  delete[] recvBuffer;  // һ��Ҫ�ر�ע���ڴ�й©
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
        ret); /* ����һ�����ӳ�����Ϣ����֪����ԭ�� */

    std::cerr << "[Client] End with ErrorLevel =  " << ret << std::endl;
    return; /* �����߳� */
  } else {
    nmComponent->PushSucessMessage(); /* ����һ�����ӳɹ���Ϣ */

    /* �߳���ѭ�� */
    while (nmComponent->GetConnectStatus() ==
           Xn::TankCraft::NetManager_Component::NET_MANAGER_ONLINE) {
      /* �����������ֱ������ */
      if (nmComponent->HasClientRequest()) {
        std::cerr << "[Client] has request to send." << std::endl;

        TcpDataList tcpDataList;
        Xn::TankCraft::NetMessageBaseDataList nmBaseDataList;

        /* �������������ԭ�ӵģ�����Ҫ�����ܽ�ʡ���� */
        nmComponent->MoveClientRequestToNetMessageBaseDataList(&nmBaseDataList);
        TcpUtils::GetTcpDataListFromNetMessageBaseDataList(&nmBaseDataList,
                                                           &tcpDataList);

        /* �����ݽ��д�� */
        // TcpData tcpDataRequest;
        std::shared_ptr<TcpData> tcpDataRequest(TcpData::AllocTcpData(__FILE__, __LINE__, false));

        TcpUtils::CompactTcpDataListToTcpDataRequest(&tcpDataList,
                                                     tcpDataRequest.get());

        /* ���ʹ��������󣬻�ȡ��Ϣ */
        int sendTime = clock();
        // TcpData tcpDataMessage;
        std::shared_ptr<TcpData> tcpDataMessage(
            TcpData::AllocTcpData(__FILE__, __LINE__));

        tcpClient.Request(tcpDataRequest.get(), tcpDataMessage.get());
        int getTime = clock();

        // TODO: �洢 Ping ֵ

        /* ����Ƿ���������Ͽ����� */
        if (tcpDataMessage->IsEnd()) {
          nmComponent->PushFailedMessage(TCP_CLIENT_DISCONNECT_FROM_SERVER);
          std::cerr << "[Client] Can not connect to server." << std::endl;
        }

        /* �ͷ���ǰ�� Request �ռ� */
        for (auto pTcpData : tcpDataList) {
          delete pTcpData;
        }
        tcpDataList.clear();

        /* ���յ�����Ϣ���н�� */
        if (!tcpDataMessage->IsEnd()) {
          TcpUtils::UnpackTcpDataMessageToTcpDataList(tcpDataMessage.get(),
                                                      &tcpDataList);

          /* �Ѳ�ֺ������������Ϣ���� */
          for (auto pTcpData : tcpDataList) {
            nmComponent->PushServerMessageTcpData(pTcpData);
          }

          for (auto pTcpData : tcpDataList) { /* �ͷ� Message �ռ� */
            delete pTcpData;
          }
          tcpDataList.clear();
        } else
          break;
      } else {
        /* ���û������ҲҪ����һ�� ping ��֤�Լ��ܹ�ˢ�µ�ͼ */
        unsigned short xVal = Utils::GetRandLongLong() % 65536;
        nmComponent->PushPingMessage(xVal);
      }

      SysUtils::Sleep(CLIENT_THREAD_SLEEP_TIME);
    }

    /* �Ͽ����� */
    tcpClient.CloseSocket();
    std::cerr << "[Client End]" << std::endl;
  }
}

void TcpUtils::CompactTcpDataListToTcpDataRequest(
    const TcpDataList* tcpDataList, TcpData* tcpDataRequest) {
  int totalLength = 4; /* ͷ����Ϣ���ֽڣ�У����Ϣ���ֽ� */
  int totalCnt = (int)tcpDataList->size();

  /* �����������Ϣ�ܳ��� */
  for (auto pTcpData : *tcpDataList) {
    totalLength += pTcpData->GetLength();
  }

  /* ���뻺���� */
  char* buf = new char[totalLength];
  int pos = 0;
  Utils::DumpUnsignedShortToBuffer(buf, pos, (unsigned short)totalCnt);
  pos += 2;

  /* ��������д������Ϣ */
  for (auto pTcpData : *tcpDataList) {
    Utils::DumpTcpDataToBuffer(buf, pos, pTcpData);
    pos += pTcpData->GetLength();
  }
  assert(pos == totalLength - 2); /* ����Ƿ�ǡ�û�ʣ����λ�� */

  /* ����У��� */
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

  /* ȡ����Ϣ���� */
  int pos = 0;
  unsigned short messageCnt =
      Utils::GetUnsignedShort(pTcpDataMessage->GetData(), pos);
  int cnt = 0;
  pos += 2;

  while (pos < messageTotalLength) {
    /* �� pos ��ʼ����Ϊ dataLength ��һ����һ����Ϣ */
    unsigned short dataLength =
        Utils::GetUnsignedShort(pTcpDataMessage->GetData(), pos + 2) + 4;

    pTcpDataMessage->DebugShow("[TcpData] ");
    assert(pos + dataLength <= messageTotalLength);

    TcpData* tcpDataNow = TcpData::AllocTcpData(__FILE__, __LINE__);
    tcpDataNow->SetData(pTcpDataMessage->GetData() + pos, dataLength);
    pTcpDataList->push_back(tcpDataNow); /* �����������ͷ� */

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
    (*nmBaseDataList)[i]->MoveDataToTcpData(tcpDataNow); /* �ƶ����� */

    /* TODO Ҫ�ǵ��ͷ� */
    tcpDataList->push_back(tcpDataNow);
  }
}
