#include "GameTcpServer.h"

#include <vector>

#include "DataParser.h"
#include "ParserRegister.h"
#include "TcpData.h"
#include "ThreadBuffer.h"

GameTcpServer::GameTcpServer(const char* ip, int port) : TcpServer(ip, port) {
  static ParserRegister* pr = nullptr;

  /* ע������������б� */
  if (pr == nullptr) {
    pr = new ParserRegister;
    pr->Register();
  }
}

void GameTcpServer::GetTcpDataResult(const TcpData* pTcpDataRecv,
                                     TcpData* pTcpDataSend, ThreadBuffer* tb,
                                     GameDatabase* Gdb) {
  /* ʹ�����ݽ���������Ϣ���н����ɶ��ֶ�������Ϣ */
  DataParser dataParser;
  IMessageList iClientRequestList;
  dataParser.Parse(pTcpDataRecv, &iClientRequestList,
                   DataParser::GetGlobalClientRequestParserList());

  /* ������Ϣ���������ı䣬�����ɷ���Ϣ */
  for (auto iClientRequest : iClientRequestList) {
    iClientRequest->Dispatch(tb, Gdb);
  }

  /* shared_ptr ����Ҫ�����ͷ����� */
  /* ��ɹ�����Ҫ�ǵ��ͷ��յ��� Request ���� */
  // for (auto iClientRequest : iClientRequestList) {
  //   delete iClientRequest;
  // }

  /* ������������������Լ���¼� */
  tb->DumpTankPosMessage(Gdb);   /* ��ȡ������̹��λ����Ϣ */
  tb->DumpBulletPosMessage(Gdb); /* ��ȡ�������ӵ�λ����Ϣ */

  tb->DumpScoreBoardMessage(Gdb); /* ��ȡ�Ʒְ���Ϣ */
  tb->DumpUserInfoMessage(Gdb);   /* ����û�������ɫӳ�� */

  tb->GetTcpDataFromDumpedMessage(pTcpDataSend); /* ��ȡҪ�����͵���Ϣ */
  tb->ClearDumpedMessage(); /* ��ջ����Ҫ���͵���Ϣ */
}
