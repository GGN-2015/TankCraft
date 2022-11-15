#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "Component.h"
#include "Math.h"

namespace Xn {

class Circular_RenderComponent;
class Square_RenderComponent;

namespace TankCraft {

#define XN_IS_OPEN_BULLET_MOVE_LERP 0

// 以x（Vector2::X）为前方向
class BulletComponent : public Component {
 public:
  BulletComponent() : Component(L"TanksFight_BulletComponent") {}

  virtual void OnStart();
  virtual void OnUpdate();

  void SetLerpTime(const Float &lerp_time);
  void SetPos(const Vector2 &pos);
  void SetTargetPos(const Vector2 &pos);
  void SetRadio(const Float &radius);
  void SetColor(const Vector3::Color &color);
  void SetColor(const Vector4::Color &color);

 private:
#if XN_IS_OPEN_BULLET_MOVE_LERP
  Float lerp_time_;
  Float though_t_;

  Vector2 start_pos_;
  Vector2 target_pos_;
#endif  // XN_IS_OPEN_BULLET_MOVE_LERP

  Circular_RenderComponent *render_component_ = nullptr;
};

}  // namespace TankCraft

}  // namespace Xn
