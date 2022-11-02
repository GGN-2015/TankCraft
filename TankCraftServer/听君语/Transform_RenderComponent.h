#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include "RenderComponent.h"

namespace Xn {

class Transform_RenderComponent : public RenderComponent {
 public:
  Transform_RenderComponent() : RenderComponent() {}

  // ͨ�� RenderComponent �̳�
  virtual void StartRender(const Vector2 &pos, const Vector2 &scale,
                           const float &rotation) override;
  virtual void OverRender() override;
};

}  // namespace Xn
