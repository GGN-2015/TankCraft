#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "RenderComponent.h"

namespace Xn {

class Line_RenderComponent : public RenderComponent {
 public:
  Line_RenderComponent()
      : RenderComponent(),
        width_(Float::ONE),
        start_point_(Vector2::ZERO),
        end_point_(Vector2::ONE),
        color_(Vector4::ONE) {}

  // 通过 RenderComponent 继承
  virtual void StartRender(const Vector2 &pos, const Vector2 &scale,
                           const float &rotation) override;
  virtual void OverRender() override;

  Float width_;
  Vector2 start_point_;
  Vector2 end_point_;
  Vector4 color_;
};

}  // namespace Xn
