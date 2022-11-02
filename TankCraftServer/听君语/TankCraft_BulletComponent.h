#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include "Component.h"
#include "Math.h"

namespace Xn {

class Circular_RenderComponent;
class Square_RenderComponent;

namespace TankCraft {

// ��x��Vector2::X��Ϊǰ����
class BulletComponent : public Component {
 public:
  BulletComponent() : Component(L"TanksFight_TankComponent") {}

  virtual void OnStart();
  virtual void OnUpdate();

  void SetLerpTime(const Float &lerp_time);
  void SetPos(const Vector2 &pos);
  void SetTargetPos(const Vector2 &pos);
  void SetRadio(const Float &radius);
  void SetColor(const Vector3 &color);
  void SetColor(const Vector4 &color);

 private:
  Float web_delay_time_;
  Float lerp_time_;
  Float though_t_;

  Vector2 start_pos_;
  Vector2 target_pos_;

  Circular_RenderComponent *render_component_ = nullptr;
};

}  // namespace TankCraft

}  // namespace Xn
