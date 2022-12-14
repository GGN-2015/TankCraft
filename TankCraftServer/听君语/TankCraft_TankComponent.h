#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "Component.h"
#include "Math.h"
#include "TankCraft_UserData.h"

namespace Xn {

class Transform_RenderComponent;
class Circular_RenderComponent;
class Square_RenderComponent;

namespace TankCraft {

class TankManagerComponent;

// 以x（Vector2::X）为前方向
class TankComponent : public Component {
  friend class TankManagerComponent;

 public:
  TankComponent() : Component(L"TanksFight_TankComponent") {}

  virtual void OnStart();
  virtual void OnUpdate();
  virtual void OnDestory();

  void SetLerpTime(const Float &lerp_time);
  void SetPos(const Vector2 &pos, const Float &rotation);
  void SetTargetPos(const Vector2 &pos, const Float &rotation);
  void SetRadio(const Float &radius);
  void SetState(const Int &state);

  void BindUser(const uint &user_id);

 private:
  Float web_delay_time_;
  Float lerp_time_;
  Float though_t_;

  Vector2 start_pos_;
  Vector2 target_pos_;
  Float start_rotation_;
  Float target_rotation_;

  Int state_;

  uint user_id_ = 0;

  XnObject *invincible_state_effect_ = nullptr;
  Circular_RenderComponent *tank_body_render_component_ = nullptr;
  Circular_RenderComponent *invincible_state_effect_render_component_ = nullptr;
  Square_RenderComponent *gun_barrel_render_component_ = nullptr;

 private:
  Transform_RenderComponent *render_component_ = nullptr;
};

}  // namespace TankCraft

}  // namespace Xn
