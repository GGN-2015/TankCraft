#include <windows.h>

#include <cassert>
#include <iostream>

#include "GameDatabase.h"
#include "GameGraph.h"
#include "GameTcpServer.h"
#include "SysUtils.h"
#include "TankCraft_NetManager_Component.h"
#include "TcpClient.h"
#include "TcpData.h"
#include "TcpServer.h"
#include "TestPrograms.h"
#include "Utils.h"

int main() {
  /* 初始化随机种子 */
  SysUtils::SRand();

  // TestTcpNetManager();
  TestGameTcpServer();
  // TestUtils();
}

void TestUtils() {
  double ans = 0, tmp;
  int N = (int)1e6;
  double mx = 0;
  double mn = 1;
  for (int i = 1; i <= N; i += 1) {
    ans += tmp = Utils::GetRandomDouble();
    mx = max(mx, tmp);
    mn = min(mn, tmp);
  }
  std::cout << "Ave = " << ans / N << ", Max = " << mx << ", Min = " << mn
            << std::endl;
}

void TestTcpServer() {
  TcpServer tcpServer("127.0.0.1", 12345);
  int ret = tcpServer.RunServer();
  if (ret != TCP_SERVER_SUC) {
    std::cerr << "tcpServer.RunServer() error " << std::endl;
    assert(false);
  }
}

void TestTcpClient() {
  TcpClient tcpClient;
  int ret = tcpClient.Connect("127.0.0.1", 12345);
  assert(ret == TCP_CLINET_CONNECT_SUC); /* 检测连接是否成功 */

  while (true) {
    std::string ans;
    std::getline(std::cin, ans);

    /* 发送请求到服务端 */
    // TcpData tcpDataSend, tcpDataGet;
    std::shared_ptr<TcpData> tcpDataSend(
        TcpData::AllocTcpData(__FILE__, __LINE__, false));

    std::shared_ptr<TcpData> tcpDataGet(
        TcpData::AllocTcpData(__FILE__, __LINE__, false));

    tcpDataSend->SetData(ans.c_str(), (int)ans.length());
    tcpClient.Request(tcpDataSend.get(), tcpDataGet.get());

    /* 输出接收到的消息 */
    std::cout << "[Server] ";
    for (int i = 0; i < tcpDataGet->GetLength(); i += 1) {
      std::cout << (unsigned int)(*(unsigned char*)&(tcpDataGet->GetData()[i]));
    }
    std::cout << std::endl;

    /* 关闭客户端 */
    if (ans == "quit") {
      break;
    }
  }
  tcpClient.CloseSocket();
}

void TestGameTcpServer() {
  GameDatabase::GetGlobalGameDatabase(); /* 实例化一个 */

  GameTcpServer gameTcpServer("0.0.0.0", 12345);
  int ret = gameTcpServer.RunServer();

  if (ret != TCP_SERVER_SUC) {
    std::cerr << "gameTcpServer.RunServer() error " << std::endl;
    assert(false);
  }
}

void TestTcpClientTool() { /* 服务端测试器 */
  TcpClient tcpClient;
  int ret = tcpClient.Connect("127.0.0.1", 12345);
  assert(ret == TCP_CLINET_CONNECT_SUC); /* 检测连接是否成功 */

  while (true) {
    int nbytes;
    std::cout << "nbytes = ";
    std::cin >> nbytes;

    if (nbytes == 0) break; /* 关闭客户端 */

    std::cout << "data = ";

    /* 输入数据 */
    char* buf = new char[nbytes];
    for (int i = 0; i < nbytes; i += 1) {
      int val;
      std::cin >> val;
      buf[i] = val;
    }

    /* 把数据填写进缓冲区中 */
    // TcpData tcpDataSend, tcpDataGet;

    std::shared_ptr<TcpData> tcpDataSend(TcpData::AllocTcpData(__FILE__, __LINE__, false));
    std::shared_ptr<TcpData> tcpDataGet(
        TcpData::AllocTcpData(__FILE__, __LINE__, false));

    tcpDataSend->SetData(buf, nbytes);
    delete[] buf;

    /* 输出服务端反馈的数据 */
    tcpClient.Request(tcpDataSend.get(), tcpDataGet.get());
    tcpDataGet->DebugShow("[Server] ");
  }
  tcpClient.CloseSocket();
}

void TestTcpNetManager() {
  Xn::TankCraft::NetManager_Component nmComponent;

  nmComponent.OnStart();
  nmComponent.ConnectToServer(L"127.0.0.1", L"12345");

  bool first = true;

  while (true) {
    /* 把数据填写进缓冲区中 */
    // TcpData tcpDataSend, tcpDataGet;

    std::shared_ptr<TcpData> tcpDataSend(
        TcpData::AllocTcpData(__FILE__, __LINE__, false));
    std::shared_ptr<TcpData> tcpDataGet(
        TcpData::AllocTcpData(__FILE__, __LINE__, false));

    if (first) {
      first = false;

      wchar_t buf[] = {0, 2, 12345};
      tcpDataSend->SetData((char*)buf, 6); /* 第一轮发送 ping */
    } else {
      int nbytes;
      std::cout << "nbytes = ";
      std::cin >> nbytes;
      if (nbytes == 0) break; /* 关闭客户端 */
      std::cout << "data = ";

      /* 输入数据 */
      char* buf = new char[nbytes];
      for (int i = 0; i < nbytes; i += 1) {
        int val;
        std::cin >> val;
        buf[i] = val;
      }

      tcpDataSend->SetData(buf, nbytes);
      delete[] buf;
    }

    /* 缓冲区送入发送队列 */
    // auto requestBufferArr = nmComponent.TryGetClientToServerMessageBuffer();
    std::unique_ptr<Xn::TankCraft::NetMessageBaseData> pnmbd1(
        new Xn::TankCraft::NetMessageBaseData);
    pnmbd1->MoveDataFrom(tcpDataSend);
    nmComponent.PushBaseDataToClientToServerMessageBuffer(std::move(pnmbd1));

    /* 获取客户收到的数据 */
    auto msgBufferArr = nmComponent.TryGetServerToClientMessageBuffer();

    while (!msgBufferArr->isEmpty()) {
      std::unique_ptr<Xn::TankCraft::NetMessageBaseData> pnmbd2(
          std::move(msgBufferArr->Pop()));

      pnmbd2->DebugShow();
      int getTime = clock();
      std::cerr << "[ClientMain] getTime = " << getTime << std::endl;
    }
  }

  nmComponent.DisConnect();
  nmComponent.OnDestory();
}

void TestGameGraph() {
  GameGraph gg;
  gg.SetSize(8, 8, 0.4);

  // TcpData tcpData;
  std::shared_ptr<TcpData> tcpData(TcpData::AllocTcpData(__FILE__, __LINE__, false));
  gg.GetTcpData(tcpData.get());

  tcpData->DebugShow("[Graph] ");
}
