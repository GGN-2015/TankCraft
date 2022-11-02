#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "Component.h"
#include "Math.h"

namespace Xn {

class Transform_RenderComponent;
class Square_RenderComponent;

namespace TankCraft {

// 以x（Vector2::X）为前方向
class MapComponent : public Component {
 public:
  MapComponent() : Component(L"TanksFight_MapComponent") {}

  virtual void OnStart();
  virtual void OnUpdate();
  virtual void OnDestory();

  void SetPos(const Vector2 &pos, const Float &scale);
  void SetTargetPos(const Vector2 &pos, const Float &scale);

 private:
  Vector2 target_pos_;
  Vector2 target_scale_;

  Square_RenderComponent *render_component_ = nullptr;
};

}  // namespace TankCraft

}  // namespace Xn
