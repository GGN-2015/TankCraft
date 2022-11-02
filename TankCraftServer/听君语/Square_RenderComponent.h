#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include "RenderComponent.h"

namespace Xn {

class Square_RenderComponent : public RenderComponent {
 public:
  Square_RenderComponent(Vector4 color)
      : RenderComponent(), rect_(0, 0, 1, 1), color_(color) {}

  // ͨ�� RenderComponent �̳�
  virtual void StartRender(const Vector2 &pos, const Vector2 &scale,
                           const float &rotation) override;
  virtual void OverRender() override;

  Vector4 rect_;
  Vector4 color_;
};

}  // namespace Xn
