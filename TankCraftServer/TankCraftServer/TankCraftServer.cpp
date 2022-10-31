#include <cassert>
#include <iostream>

#include "GameTcpServer.h"
#include "TcpData.h"
#include "TcpClient.h"
#include "TcpServer.h"
#include "TestPrograms.h"

int main()
{
    /* 初始化随机种子 */
    srand((unsigned int)time(NULL));

    TestGameTcpServer();
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
        TcpData tcpDataSend, tcpDataGet;
        tcpDataSend.SetData(ans.c_str(), ans.length());
        tcpClient.Request(&tcpDataSend, &tcpDataGet);

        /* 输出接收到的消息 */
        std::cout << "[Server] ";
        for (int i = 0; i < tcpDataGet.GetLength(); i += 1) {
            std::cout << (unsigned int)(*(unsigned char*)&tcpDataGet.GetData()[i]);
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
    GameTcpServer gameTcpServer("127.0.0.1", 12345);
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
        int nbytes; std::cout << "nbytes = "; std::cin >> nbytes;

        if (nbytes == 0) break; /* 关闭客户端 */

        std::cout << "data = ";

        /* 输入数据 */
        char* buf = new char[nbytes];
        for (int i = 0; i < nbytes; i += 1) {
            int val; std::cin >> val;
            buf[i] = val;
        }

        /* 把数据填写进缓冲区中 */
        TcpData tcpDataSend, tcpDataGet;
        tcpDataSend.SetData(buf, nbytes);
        delete[] buf;

        /* 输出服务端反馈的数据 */
        tcpClient.Request(&tcpDataSend, &tcpDataGet);
        tcpDataGet.DebugShow("[Server] ");
    }
    tcpClient.CloseSocket();
}
