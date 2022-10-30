#include <iostream>

#include "TcpServer.h"

int main()
{
    TcpServer tcpServer("127.0.0.1", 12345);
    int ret = tcpServer.RunServer();
    if (ret != TCP_SERVER_SUC) {
        std::cerr << "tcpServer.RunServer() error " << std::endl;
    }
}
