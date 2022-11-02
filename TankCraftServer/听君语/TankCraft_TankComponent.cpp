#include "TankCraft_TankComponent.h"
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "Circular_RenderComponent.h"
#include "ObjectManager-XnObject.h"
#include "听君语.h"

using namespace Xn;
using namespace TankCraft;

void TankComponent::OnStart() {
  render_component_ =
      (Circular_RenderComponent *)GetXnObject()->SetRenderComponent(
          std::make_unique<Circular_RenderComponent>());

  though_t_ = 0;

#if _DEBUG && 1
  SetPos(Vector2::Random({0, 0}, {10, 10}),
         Vector2::Random(Vector2::X, Vector2::Y));
  SetColor(Vector3::Random(Vector3(0.6, 0.6, 0.6), Vector3::ONE));
  SetLerpTime(Float::Random());
  SetRadio(Float::Random(0.4, 0.8));
#endif  // Test
}
void TankComponent::OnUpdate() {
  though_t_ += 听君语::Get().GetDeltaTime();
#if _DEBUG && 1
  if (though_t_ >= lerp_time_) {
    SetTargetPos(Vector2::Random({0, 0}, {10, 10}),
                 Vector2::Random(Vector2::X, Vector2::Y));
  } else {
    auto t = though_t_.ScaleFromTo(0, lerp_time_, 0, 1);
    GetXnObject()->pos_ = Vector2::Lerp(start_pos_, target_pos_, t);
    GetXnObject()->rotation_ =
        Float::Lerp(start_rotation_, target_rotation_, t);
  }
#else
  GetXnObject()->pos_ = Vector2::Lerp(start_pos_, target_pos_, though_t_);
  GetXnObject()->rotation_ =
      Float::Lerp(start_rotation_, target_rotation_, though_t_);
#endif  // Test
}
void TankComponent::OnDestory() {}

void TankComponent::SetLerpTime(const Float &lerp_time) {
  lerp_time_ = lerp_time;
}
void TankComponent::SetPos(const Vector2 &pos, const Float &rotation) {
  GetXnObject()->pos_ = start_pos_ = target_pos_ = pos;
  GetXnObject()->rotation_ = start_rotation_ = target_rotation_ = rotation;
}
void TankComponent::SetPos(const Vector2 &pos, const Vector2 &direction) {
  GetXnObject()->pos_ = start_pos_ = target_pos_ = pos;
  // TODO direction to rotation
}
void TankComponent::SetTargetPos(const Vector2 &pos, const Float &rotation) {
  though_t_ = 0;
  start_pos_ = target_pos_;
  start_rotation_ = target_rotation_;
  target_pos_ = pos;
  target_rotation_ = rotation;
}
void TankComponent::SetTargetPos(const Vector2 &pos, const Vector2 &direction) {
  // TODO direction to rotation
  SetTargetPos(pos, 0);
}
void TankComponent::SetRadio(const Float &radius) {
  render_component_->radius_ = radius;
}
void TankComponent::SetColor(const Vector3 &color) {
  SetColor(Vector4(color.x, color.y, color.z, 1));
}
void TankComponent::SetColor(const Vector4 &color) {
  render_component_->color_ = color;
}

void TankComponent::BindUser() {
  // TODO 绑定用户信息
}
