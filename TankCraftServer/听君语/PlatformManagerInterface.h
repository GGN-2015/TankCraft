#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include "Math.h"

namespace Xn {

enum class WindowZorder {
  Bottom,     // ����������Z��ĵײ�
  Top,        // ����������Z��Ķ���
  NoTopmost,  // �������������зǶ��㴰��֮�ϣ��Ѿ�ʹ�Ƕ��㴰�ڲ�������
  Topmost,    // �������������зǶ��㴰��֮�ϣ���ʹδ������
  Count
};

// ƽ̨�޹ػ�������
// Լ����
//   - �ڵ����ڲ�����������ǰ��Ӧ������ִ��Run����һ�Σ�
//     ����ͨ��Run���������ڳ������ǰ����
class PlatformManagerInterface {
 public:
  // ��ʼ����
  virtual int Run() = 0;

  virtual std::wstring GetWindowTitle() = 0;
  virtual Vector2Int GetPos() = 0;
  virtual int GetWidth() = 0;
  virtual int GetHeight() = 0;
  // ȡ�õ�ǰ�����Ƿ���ȫ��״̬
  virtual const bool IsFullscreen() = 0;

  // ���õ�ǰ�����Ƿ���ȫ��״̬
  virtual void SetFullscreen(bool is_fullscreen) = 0;
  // ���ô�������Ļ�ռ��Z��
  virtual void SetWindowZorder(const WindowZorder &window_zorder) = 0;
};

}  // namespace Xn
