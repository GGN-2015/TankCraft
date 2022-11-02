#pragma once

#include <cassert>
#include <memory>
#include <queue>
#include <thread>

#include "Component.h"
#include "MyMutex.h"

namespace Xn {

typedef unsigned int uint;
typedef unsigned short uint16;
typedef unsigned char byte;
typedef wchar_t wchar;

class TcpDataList;

namespace TankCraft {

typedef std::vector<wchar_t> WcharList;

struct NetMessageBaseData {
  uint length;    // �ǰ�wchar�ĳ���
  WcharList data; /* ʱ����ۿ��ܻ�Ƚϸ� */
};

class NetMessageBaseDataBuffer {
 public:
  // ��ǰ���ݻ����Ƿ�Ϊ��
  inline bool isEmpty() const { return datas_.empty(); }

  // ����һ������
  inline void Push(std::unique_ptr<NetMessageBaseData> data) {
    datas_.push(std::move(data));
  }

  // ȡ��һ������
  inline std::unique_ptr<NetMessageBaseData> Pop() {
    if (isEmpty()) {
      return nullptr;
    } else {
      std::unique_ptr<NetMessageBaseData> data = std::move(datas_.front());
      datas_.pop();
      return data;
    }
  }

  /* ������� BufferArray */
  inline void Clear() {
    while (!isEmpty()) {
      Pop();
    }
  }

 private:
  std::queue<std::unique_ptr<NetMessageBaseData>> datas_;
};

class NetManager_Component : public Component {
 public:
  static const uint FROM_SERVER_DATA_BUFFER_COUNNT_ = 2;
  static const uint FROM_CLIENT_DATA_BUFFER_COUNNT_ = 2;

  static const int NET_MANAGER_OFFLINE = (0); /* mConnectStatus �Ŀ���ȡֵ */
  static const int NET_MANAGER_ONLINE = (1);

 public:
  NetManager_Component()
      : Component(L"NetManager_Component"),
        mConnectStatus(NET_MANAGER_OFFLINE) {
    from_server_datas_buffer_index_ = 0;
    from_client_datas_buffer_index_ = 0;
  }

  // ͨ�� Component �̳�
  virtual void OnStart() override;
  virtual void OnDestory() override;

 public:
  // ���ӵ�������
  // ����:
  //   - 0     : �ɹ�
  //   - not 0 : ʧ��
  uint ConnectToServer(const uint16& ipV4_1, const uint16& ipV4_2,
                       const uint16& ipV4_3, const uint16& ipV4_4,
                       const uint& port);

  // ���ӵ�������
  // ����:
  //   - ipV4 [wstring] : ��ʽΪ [L"1:2:3:4"]��
  //         1��2��3��4�ֱ����0~255�е�һ��ֵ�Ŀ��ַ�����ʽ
  //   - port [wstring] : ��ʽΪ [L"10086"]
  // ����:
  //   - 0     : �ɹ�
  //   - not 0 : ʧ��
  uint ConnectToServer(std::wstring ipV4, std::wstring port);

  // �ر�����
  void DisConnect();

  // ȡ�÷��������ͻ��˵���Ϣ����
  // Լ��:
  //   - ȡ�û����ǰһ��ȡ�õĻ���ָ�벻��ʹ��
  // ����:
  //   - nullptr     : ��ȡʧ�ܣ�������Ϊ����ʲô��
  //   - not nullptr : ��ȡ�ɹ���ָ��NetMessageBaseDataBuffer��ָ��
  NetMessageBaseDataBuffer* const& TryGetServerToClientMessageBuffer();

  // ȡ�ÿͻ��˵�����������Ϣ����
  // Լ��:
  //   - ȡ�û����ǰһ��ȡ�õĻ���ָ�벻��ʹ��
  // ����:
  //   - nullptr     : ��ȡʧ�ܣ�������Ϊ����ʲô��
  //   - not nullptr : ��ȡ�ɹ���ָ��NetMessageBaseDataBuffer��ָ��
  NetMessageBaseDataBuffer* const& TryGetClientToServerMessageBuffer();

 public:
  void PushFailedMessage(int ret); /* ԭ�ӣ����ͳ�����Ϣ */
  void PushSucessMessage();        /* ԭ�ӣ����ͳɹ���Ϣ */
  int GetConnectStatus() const;    /* ԭ�ӣ���ȡ����״̬ */

  bool HasClientRequest() const; /* ԭ�ӣ����ͻ����Ƿ�Ҫ�����˷�����Ϣ */
  void MoveClientRequestToTcpDataList(
      TcpDataList* tcpDataList); /* ԭ�ӵ�����ת�� */

 private:
  /* ԭ�ӣ��൱��һ�����Է���������Ϣ������ʵ�����Ǳ������͵� */
  void PushToFromServerList(std::unique_ptr<NetMessageBaseData> nmData);

 protected:
  void Lock() const; /* ����/���� */
  void Unlock() const;

 private:
  int mConnectStatus; /* ���̸߳�����һ�����˳� */

  uint from_server_datas_buffer_index_; /* ����� index ָ���ǵ�ǰ��������ʹ�õ�
                                           index */
  uint from_client_datas_buffer_index_;
  NetMessageBaseDataBuffer from_server_datas_buffers_
      [FROM_SERVER_DATA_BUFFER_COUNNT_];  // �����Է���˵Ļ���
  NetMessageBaseDataBuffer from_client_datas_buffers_
      [FROM_CLIENT_DATA_BUFFER_COUNNT_];  // �����Կͻ��˵Ļ���

  MyMutex mMyMutex;                     /* ���ݻ����� */
  std::thread* mClientThread = nullptr; /* �ͻ�ͨ���߳� */
};

}  // namespace TankCraft

}  // namespace Xn