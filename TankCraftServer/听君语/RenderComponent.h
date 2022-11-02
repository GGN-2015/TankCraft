#pragma once
//*********************************************************
//
// ”…°∏¬Ì’—°π±‡–¥
//
//*********************************************************

#include "Component.h"
#include "RenderManagerInterface.h"

namespace Xn {

class RenderComponent : public Component {
 public:
  RenderComponent();

  virtual void StartRender(const Vector2 &pos, const Vector2 &scale,
                           const float &rotation) = 0;
  virtual void OverRender() = 0;

 public:
};

}  // namespace Xn
