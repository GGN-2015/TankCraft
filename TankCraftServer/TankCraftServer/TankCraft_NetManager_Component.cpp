//#include "TankCraft_NetManager_Component.h"
//
//#include <thread>
//
//#include "TcpUtils.h"
//
//using namespace Xn;
//using namespace Xn::TankCraft;
//
//void Xn::TankCraft::NetManager_Component::OnStart() {
//  from_server_datas_buffer_index_ = 0;
//  from_client_datas_buffer_index_ = 0;
//
//  mClientThread = nullptr;
//
//  /* 清空所有服务端数据的队列 */
//  for (auto& bufferArray : from_server_datas_buffers_) {
//    bufferArray.Clear();
//  }
//
//  /* 清空所有客户端数据的队列 */
//  for (auto& bufferArray : from_client_datas_buffers_) {
//    bufferArray.Clear();
//  }
//}
//
//void Xn::TankCraft::NetManager_Component::OnDestory() {
//  /* 如果当前仍在和服务器的连接当中 */
//  if (mConnectStatus != NET_MANAGER_OFFLINE) {
//    mConnectStatus = NET_MANAGER_OFFLINE;
//    mClientThread->join(); /* 等待线程退出 */
//  }
//  mClientThread = nullptr;
//}
//
///* 直接调用 wstring 版本的接口即可 */
//uint Xn::TankCraft::NetManager_Component::ConnectToServer(const uint16& ipV4_1,
//                                                          const uint16& ipV4_2,
//                                                          const uint16& ipV4_3,
//                                                          const uint16& ipV4_4,
//                                                          const uint& port) {
//  std::wstring wsIp = std::to_wstring(ipV4_1) + L"." + std::to_wstring(ipV4_2) +
//                      L"." + std::to_wstring(ipV4_3) + L"." +
//                      std::to_wstring(ipV4_4); /* 例如：192.168.100.123 */
//
//  std::wstring wsPort = std::to_wstring(port);
//  return ConnectToServer(wsIp, wsPort);
//}
//
//uint Xn::TankCraft::NetManager_Component::ConnectToServer(std::wstring ipV4,
//                                                          std::wstring port) {
//  /* 将 wchar_t* 转化为 char* */
//  char ipBuf[20] = {};
//  int pos = 0;
//  for (auto wchar : ipV4) {
//    ipBuf[pos] = *(char*)&wchar; /* 获取两个字节中的第一个字节 */
//    pos += 1;
//  }
//
//  int iPort = std::stoi(port);
//  mClientThread =
//      new std::thread(TcpUtils::ClientThreadFunction, ipBuf, iPort, this);
//  mClientThread->detach();
//
//  return 0;
//}
//
//void Xn::TankCraft::NetManager_Component::DisConnect() {
//  mConnectStatus = NET_MANAGER_OFFLINE; /* 关闭客户端的连接 */
//}
//
//NetMessageBaseDataBuffer* const&
//Xn::TankCraft::NetManager_Component::TryGetServerToClientMessageBuffer() {
//  Lock();
//
//  /* from_server_datas_buffer_index_ 是当前的服务线程正在占用的 buffer index */
//  NetMessageBaseDataBuffer* foo =
//      &from_server_datas_buffers_[from_server_datas_buffer_index_];
//  from_server_datas_buffer_index_ ^= 1;
//
//  Unlock();
//  return foo;
//}
//
//NetMessageBaseDataBuffer* const&
//Xn::TankCraft::NetManager_Component::TryGetClientToServerMessageBuffer() {
//  Lock();
//
//  /* from_client_datas_buffer_index_ 是当前的服务线程正在占用的 buffer index */
//  NetMessageBaseDataBuffer* bufferFrom =
//      &from_client_datas_buffers_[from_client_datas_buffer_index_ ^ 1];
//  NetMessageBaseDataBuffer* bufferTo =
//      &from_client_datas_buffers_[from_client_datas_buffer_index_];
//
//  /* 将当前队列中的数据放到目标队列中 */
//  while (!bufferFrom->isEmpty()) {
//    std::unique_ptr<NetMessageBaseData> data = bufferFrom->Pop();
//    bufferTo->Push(std::move(data));
//  }
//
//  Unlock();
//
//  /* 一定会返回一个空队列 */
//  return bufferFrom;
//}
//
//void Xn::TankCraft::NetManager_Component::PushFailedMessage(int ret) {
//  /* 新申请一个数据缓存 */
//  std::unique_ptr<Xn::TankCraft::NetMessageBaseData> newMsg(
//      new Xn::TankCraft::NetMessageBaseData);
//
//  /* GGN: 我不喜欢写死，但是我还是写死了，懒得写一个常量管理器了 */
//  newMsg->length = 3;
//  newMsg->data.push_back(65535);
//  newMsg->data.push_back(2);
//  newMsg->data.push_back(*(wchar_t*)&ret); /* 只返回低两位，本方法不可移植 */
//
//  /* 相当于从服务端发送来了一条数据 */
//  PushToFromServerList(std::move(newMsg));
//}
//
//void Xn::TankCraft::NetManager_Component::PushSucessMessage() {
//  PushFailedMessage(0); /* 错误码为零，表示成功 */
//}
//
//int Xn::TankCraft::NetManager_Component::GetConnectStatus() const {
//  /* 读数据时候加锁 */
//  Lock();
//  int ans = mConnectStatus;
//  Unlock();
//
//  return ans;
//}
//
//bool Xn::TankCraft::NetManager_Component::HasClientRequest() const {
//  Lock();
//  bool ans = false;
//  for (auto& bufferArray : from_client_datas_buffers_) {
//    ans |= (!bufferArray.isEmpty()); /* 只要有一个非空，就非空 */
//  }
//  Unlock();
//
//  return ans;
//}
//
//void Xn::TankCraft::NetManager_Component::PushToFromServerList(
//    std::unique_ptr<NetMessageBaseData> nmData) {
//  /* 将数据移动到对应的队列中 */
//  Lock();
//  from_server_datas_buffers_[from_server_datas_buffer_index_].Push(
//      std::move(nmData));
//  Unlock();
//}
//
//void Xn::TankCraft::NetManager_Component::Lock() const { mMyMutex.lock(); }
//
//void Xn::TankCraft::NetManager_Component::Unlock() const { mMyMutex.unlock(); }
