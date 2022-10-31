#include <cassert>
#include <iostream>

#include "TcpData.h"
#include "TcpClient.h"
#include "TcpServer.h"
#include "TestPrograms.h"

int main()
{
    /* 初始化随机种子 */
    srand(time(NULL));

    TestTcpClient();
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
            std::cout << tcpDataGet.GetData()[i];
        }
        std::cout << std::endl;

        /* 关闭客户端 */
        if (ans == "quit") {
            break;
        }
    }
    tcpClient.CloseSocket();
}
