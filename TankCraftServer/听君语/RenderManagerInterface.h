#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include "Math.h"
#include "typedef.h"

namespace Xn {

class RenderComponent;

enum class WindowShowState {
  Minimized,   // ��С��
  Normal,      // �������ڴ�С
  Fullscreen,  // ȫ��
  Count
};

class RenderManagerInterface {
 public:
  virtual void StartRender() = 0;
  virtual void EndRender() = 0;
  virtual void RenderRectSet(const Vector4 &rect) = 0;
  virtual void RenderRectOver() = 0;
  virtual void RenderTransformSet(const Vector2 &pos, const Vector2 &scale,
                                  const float &rotation) = 0;
  virtual void RenderTransformOver() = 0;
  virtual void OnDestory() = 0;

  virtual uint GetHeight() = 0;
  virtual uint GetWidth() = 0;

  virtual void SetWidthAndHeight(const uint &width, const uint &height) = 0;
};

}  // namespace Xn
