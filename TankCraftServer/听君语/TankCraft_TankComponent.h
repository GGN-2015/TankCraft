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

namespace TankCraft {

// 以x（Vector2::X）为前方向
class TankComponent : public Component {
 public:
  TankComponent() : Component(L"TanksFight_TankComponent") {}

  virtual void OnStart();
  virtual void OnUpdate();
  virtual void OnDestory();

  void SetLerpTime(const Float &lerp_time);
  void SetPos(const Vector2 &pos, const Float &rotation);
  void SetPos(const Vector2 &pos, const Vector2 &direction);
  void SetTargetPos(const Vector2 &pos, const Float &rotation);
  void SetTargetPos(const Vector2 &pos, const Vector2 &direction);
  void SetRadio(const Float &radius);
  void SetColor(const Vector3 &color);
  void SetColor(const Vector4 &color);

  void BindUser();  // TODO

 private:
  Float lerp_time_;
  Float though_t_;

  Vector2 start_pos_;
  Vector2 target_pos_;
  Float start_rotation_;
  Float target_rotation_;

  Circular_RenderComponent *render_component_ = nullptr;
};

}  // namespace TanksFight

}  // namespace Xn
