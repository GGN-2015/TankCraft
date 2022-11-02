#pragma once

#include <memory>
#include <queue>

#include "Component.h"

namespace Xn {

typedef unsigned int uint;
typedef unsigned short uint16;
typedef unsigned char byte;
typedef wchar_t wchar;

namespace TanksFight {

struct NetMessageBaseData {
  uint length;  // �ǰ�wchar�ĳ���
  std::unique_ptr<wchar> data;
};

class NetMessageBaseDataBuffer {
 public:
  // ��ǰ���ݻ����Ƿ�Ϊ��
  bool isEmpty();
  // ����һ������
  void Push(std::unique_ptr<NetMessageBaseData> data) {
    datas_.push(std::move(data));
  }
  // ȡ��һ������
  std::unique_ptr<NetMessageBaseData> Pop() {
    std::unique_ptr<NetMessageBaseData> data = std::move(datas_.front());
    datas_.pop();
    return data;
  }

 private:
  std::queue<std::unique_ptr<NetMessageBaseData>> datas_;
};

class NetManager_Component : public Component {
 public:
  static const uint from_server_datas_buffer_count_;
  static const uint from_client_datas_buffer_count_;

 public:
  NetManager_Component() : Component(L"NetManager_Component") {}

  // ͨ�� Component �̳�
  virtual void OnStart() override;
  virtual void OnDestory() override;

 public:
  // ���ӵ�������
  // ����:
  //   - 0     : �ɹ�
  //   - not 0 : ʧ��
  uint ConnectToServer(const uint16 &ipV4_1, const uint16 &ipV4_2,
                       const uint16 &ipV4_3, const uint16 &ipV4_4,
                       const uint &port);
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
  NetMessageBaseDataBuffer *const &TryGetServerToClientMessageBuffer();
  // ȡ�ÿͻ��˵�����������Ϣ����
  // Լ��:
  //   - ȡ�û����ǰһ��ȡ�õĻ���ָ�벻��ʹ��
  // ����:
  //   - nullptr     : ��ȡʧ�ܣ�������Ϊ����ʲô��
  //   - not nullptr : ��ȡ�ɹ���ָ��NetMessageBaseDataBuffer��ָ��
  NetMessageBaseDataBuffer *const &TryGetServerToClientMessageBuffer();

 private:
  uint from_server_datas_buffer_index_ = 0;
  uint from_client_datas_buffer_index_ = 0;
  NetMessageBaseDataBuffer from_server_datas_buffers_[2];  // �����Է���˵Ļ���
  NetMessageBaseDataBuffer from_client_datas_buffers_[2];  // �����Կͻ��˵Ļ���
};

}  // namespace TanksFight

}  // namespace Xn
