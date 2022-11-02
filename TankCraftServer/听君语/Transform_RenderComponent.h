#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "RenderComponent.h"

namespace Xn {

class Transform_RenderComponent : public RenderComponent {
 public:
  Transform_RenderComponent() : RenderComponent() {}

  // 通过 RenderComponent 继承
  virtual void StartRender(const Vector2 &pos, const Vector2 &scale,
                           const float &rotation) override;
  virtual void OverRender() override;
};

}  // namespace Xn
