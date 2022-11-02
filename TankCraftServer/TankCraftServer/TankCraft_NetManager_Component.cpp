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
//  /* ������з�������ݵĶ��� */
//  for (auto& bufferArray : from_server_datas_buffers_) {
//    bufferArray.Clear();
//  }
//
//  /* ������пͻ������ݵĶ��� */
//  for (auto& bufferArray : from_client_datas_buffers_) {
//    bufferArray.Clear();
//  }
//}
//
//void Xn::TankCraft::NetManager_Component::OnDestory() {
//  /* �����ǰ���ںͷ����������ӵ��� */
//  if (mConnectStatus != NET_MANAGER_OFFLINE) {
//    mConnectStatus = NET_MANAGER_OFFLINE;
//    mClientThread->join(); /* �ȴ��߳��˳� */
//  }
//  mClientThread = nullptr;
//}
//
///* ֱ�ӵ��� wstring �汾�Ľӿڼ��� */
//uint Xn::TankCraft::NetManager_Component::ConnectToServer(const uint16& ipV4_1,
//                                                          const uint16& ipV4_2,
//                                                          const uint16& ipV4_3,
//                                                          const uint16& ipV4_4,
//                                                          const uint& port) {
//  std::wstring wsIp = std::to_wstring(ipV4_1) + L"." + std::to_wstring(ipV4_2) +
//                      L"." + std::to_wstring(ipV4_3) + L"." +
//                      std::to_wstring(ipV4_4); /* ���磺192.168.100.123 */
//
//  std::wstring wsPort = std::to_wstring(port);
//  return ConnectToServer(wsIp, wsPort);
//}
//
//uint Xn::TankCraft::NetManager_Component::ConnectToServer(std::wstring ipV4,
//                                                          std::wstring port) {
//  /* �� wchar_t* ת��Ϊ char* */
//  char ipBuf[20] = {};
//  int pos = 0;
//  for (auto wchar : ipV4) {
//    ipBuf[pos] = *(char*)&wchar; /* ��ȡ�����ֽ��еĵ�һ���ֽ� */
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
//  mConnectStatus = NET_MANAGER_OFFLINE; /* �رտͻ��˵����� */
//}
//
//NetMessageBaseDataBuffer* const&
//Xn::TankCraft::NetManager_Component::TryGetServerToClientMessageBuffer() {
//  Lock();
//
//  /* from_server_datas_buffer_index_ �ǵ�ǰ�ķ����߳�����ռ�õ� buffer index */
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
//  /* from_client_datas_buffer_index_ �ǵ�ǰ�ķ����߳�����ռ�õ� buffer index */
//  NetMessageBaseDataBuffer* bufferFrom =
//      &from_client_datas_buffers_[from_client_datas_buffer_index_ ^ 1];
//  NetMessageBaseDataBuffer* bufferTo =
//      &from_client_datas_buffers_[from_client_datas_buffer_index_];
//
//  /* ����ǰ�����е����ݷŵ�Ŀ������� */
//  while (!bufferFrom->isEmpty()) {
//    std::unique_ptr<NetMessageBaseData> data = bufferFrom->Pop();
//    bufferTo->Push(std::move(data));
//  }
//
//  Unlock();
//
//  /* һ���᷵��һ���ն��� */
//  return bufferFrom;
//}
//
//void Xn::TankCraft::NetManager_Component::PushFailedMessage(int ret) {
//  /* ������һ�����ݻ��� */
//  std::unique_ptr<Xn::TankCraft::NetMessageBaseData> newMsg(
//      new Xn::TankCraft::NetMessageBaseData);
//
//  /* GGN: �Ҳ�ϲ��д���������һ���д���ˣ�����дһ�������������� */
//  newMsg->length = 3;
//  newMsg->data.push_back(65535);
//  newMsg->data.push_back(2);
//  newMsg->data.push_back(*(wchar_t*)&ret); /* ֻ���ص���λ��������������ֲ */
//
//  /* �൱�ڴӷ���˷�������һ������ */
//  PushToFromServerList(std::move(newMsg));
//}
//
//void Xn::TankCraft::NetManager_Component::PushSucessMessage() {
//  PushFailedMessage(0); /* ������Ϊ�㣬��ʾ�ɹ� */
//}
//
//int Xn::TankCraft::NetManager_Component::GetConnectStatus() const {
//  /* ������ʱ����� */
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
//    ans |= (!bufferArray.isEmpty()); /* ֻҪ��һ���ǿգ��ͷǿ� */
//  }
//  Unlock();
//
//  return ans;
//}
//
//void Xn::TankCraft::NetManager_Component::PushToFromServerList(
//    std::unique_ptr<NetMessageBaseData> nmData) {
//  /* �������ƶ�����Ӧ�Ķ����� */
//  Lock();
//  from_server_datas_buffers_[from_server_datas_buffer_index_].Push(
//      std::move(nmData));
//  Unlock();
//}
//
//void Xn::TankCraft::NetManager_Component::Lock() const { mMyMutex.lock(); }
//
//void Xn::TankCraft::NetManager_Component::Unlock() const { mMyMutex.unlock(); }