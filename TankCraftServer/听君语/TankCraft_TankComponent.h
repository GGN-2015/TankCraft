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

  void BindUser(const UserData *const &user_data);

 private:
  Float web_delay_time_;
  Float lerp_time_;
  Float though_t_;

  Vector2 start_pos_;
  Vector2 target_pos_;
  Float start_rotation_;
  Float target_rotation_;

  Int state_;

  const UserData *user_data_ = nullptr;

 private:
  Circular_RenderComponent *render_component_ = nullptr;
  Square_RenderComponent *gun_barrel_render_component_ = nullptr;
};

}  // namespace TankCraft

}  // namespace Xn
