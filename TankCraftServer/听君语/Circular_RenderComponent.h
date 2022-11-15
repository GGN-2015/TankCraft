#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "RenderComponent.h"

namespace Xn {

class Circular_RenderComponent : public RenderComponent {
 public:
  Circular_RenderComponent()
      : RenderComponent(), radius_(1), color_(Vector4::ONE.asColor) {}

  // 通过 RenderComponent 继承
  virtual void StartRender(const Vector2 &pos, const Vector2 &scale,
                           const float &rotation) override;
  virtual void OverRender() override;

  Float radius_;
  Vector4::Color color_;
};

}  // namespace Xn
