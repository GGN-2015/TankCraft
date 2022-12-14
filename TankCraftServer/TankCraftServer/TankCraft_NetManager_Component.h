#pragma once

#include <cassert>
#include <memory>
#include <queue>
#include <thread>

#include "Component.h"
#include "MyMutex.h"

/* 不写到这我的程序可能兼容不了 */
class TcpData;
typedef std::vector<std::shared_ptr<TcpData> > TcpDataList;

namespace Xn {

typedef unsigned int uint;
typedef unsigned short uint16;
typedef unsigned char byte;
typedef wchar_t wchar;

namespace TankCraft {

struct NetMessageBaseData {
  uint length;   // 是按wchar的长度
  wchar_t* data; /* 时间代价可能会比较高 */

  NetMessageBaseData() : length(0), data(nullptr){};
  ~NetMessageBaseData() { FreeData(); }     /* 析构函数 */
  void MoveDataFrom(std::shared_ptr<TcpData> pTcpData); /* 转移构造 */
  void MoveDataToTcpData(
      std::shared_ptr<TcpData> tcpData) volatile; /* 将数据转移出去 */
  void FreeData();                          /* 安全地清空数据 */
  void SetData(wchar_t* nData, int nLen); /* 释放原先的数据，设置新数据 */

  void DebugShow() const; /* 仅在调试时使用 */
};

/* 数据缓冲区 */
typedef std::vector<std::unique_ptr<volatile NetMessageBaseData>>
    NetMessageBaseDataList;

class NetMessageBaseDataBuffer {
 public:
  bool isEmpty() const;  // 当前数据缓冲是否为空
  void Push(std::unique_ptr<NetMessageBaseData> data);  // 加入一个数据
  std::unique_ptr<NetMessageBaseData> Pop();            // 取出一个数据
  void Clear(); /* 清空整个 BufferArray */
 private:
  std::queue<std::unique_ptr<NetMessageBaseData>> datas_;
};

class NetManager_Component : public Component {
 public:
  static const uint FROM_SERVER_DATA_BUFFER_COUNNT_ = 2;
  static const uint FROM_CLIENT_DATA_BUFFER_COUNNT_ = 2;

  static const int NET_MANAGER_OFFLINE = (0); /* mConnectStatus 的可能取值 */
  static const int NET_MANAGER_ONLINE = (1);

 public:
  NetManager_Component();

  // 通过 Component 继承
  virtual void OnStart() override;
  virtual void OnDestory() override;

 public:
  // 连接到服务器
  // 返回:
  //   - 0     : 成功
  //   - not 0 : 失败
  uint ConnectToServer(const uint16& ipV4_1, const uint16& ipV4_2,
                       const uint16& ipV4_3, const uint16& ipV4_4,
                       const uint& port);

  // 连接到服务器
  // 参数:
  //   - ipV4 [wstring] : 格式为 [L"1:2:3:4"]，
  //         1、2、3、4分别代表0~255中的一个值的宽字符串格式
  //   - port [wstring] : 格式为 [L"10086"]
  // 返回:
  //   - 0     : 成功
  //   - not 0 : 失败
  uint ConnectToServer(std::wstring ipV4, std::wstring port);

  // 关闭连接
  void DisConnect();

  // 取得服务器到客户端的消息缓冲
  // 约定:
  //   - 取得缓冲后，前一次取得的缓冲指针不再使用
  // 返回:
  //   - nullptr     : 获取失败，可能因为加锁什么的
  //   - not nullptr : 获取成功，指向NetMessageBaseDataBuffer的指针
  NetMessageBaseDataBuffer* TryGetServerToClientMessageBuffer();

  // 取得客户端到服务器的消息缓冲
  // 约定:
  //   - 取得缓冲后，前一次取得的缓冲指针不再使用
  // 返回:
  //   - nullptr     : 获取失败，可能因为加锁什么的
  //   - not nullptr : 获取成功，指向NetMessageBaseDataBuffer的指针
  
  // NetMessageBaseDataBuffer* TryGetClientToServerMessageBuffer();
  void PushBaseDataToClientToServerMessageBuffer(
      std::unique_ptr<NetMessageBaseData> data); /* 写入一个数据 */
  
 public:
  void Lock() const; /* 加锁/解锁 */
  void Unlock() const;

  void PushPingMessage(unsigned short xVal); /* 原子发送 Ping 消息*/
  void PushFailedMessage(int ret);           /* 原子：推送出错消息 */
  void PushSucessMessage();                  /* 原子：推送成功消息 */
  int GetConnectStatus() const;              /* 原子：获取连接状态 */

  bool HasClientRequest() const; /* 不是原子：检测客户端是否要想服务端发送消息 */
  void MoveClientRequestToNetMessageBaseDataList(
      NetMessageBaseDataList* nmBaseDataList); /* 原子：数据转移 */

  void PushServerMessageTcpData(std::shared_ptr<TcpData> pTcpData); /* 原子：向来自服务器的消息序列追加数据
                                                                     */

 private:
  /* 原子：相当于一条来自服务器的消息，但是实际上是本机推送的 */
  void PushToFromServerList(std::unique_ptr<NetMessageBaseData> nmData);

 private:
  int mConnectStatus; /* 子线程根据这一标致退出 */

  uint
      from_server_datas_buffer_index_; /* 这里的 index 指的是当前本类正在使用的
                                                                                  index */
  uint from_client_datas_buffer_index_;
  NetMessageBaseDataBuffer from_server_datas_buffers_
      [FROM_SERVER_DATA_BUFFER_COUNNT_];  // 发送自服务端的缓冲
  NetMessageBaseDataBuffer from_client_datas_buffers_
      [FROM_CLIENT_DATA_BUFFER_COUNNT_];  // 发送自客户端的缓冲

  MyMutex mMyMutex;                     /* 数据互斥锁 */
  std::thread* mClientThread = nullptr; /* 客户通信线程 */
};

}  // namespace TankCraft

}  // namespace Xn
