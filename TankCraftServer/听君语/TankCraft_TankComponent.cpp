#include "TankCraft_TankComponent.h"
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "Circular_RenderComponent.h"
#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "Square_RenderComponent.h"
#include "听君语.h"

using namespace Xn;
using namespace TankCraft;

void TankComponent::OnStart() {
  render_component_ =
      (Circular_RenderComponent *)GetXnObject()->SetRenderComponent(
          std::make_unique<Circular_RenderComponent>());

  though_t_ = 0;

  // 设置炮管位置
  gun_barrel_render_component_ =
      (Square_RenderComponent *)听君语::Get()
          .GetObjectManager()
          ->CreateXnObject(Vector2::ZERO, GetXnObject())
          ->SetRenderComponent(std::make_unique<Square_RenderComponent>(
              Vector4(162.f / 255.f, 168.f / 255.f, 160.f / 255.f, 1)));
  gun_barrel_render_component_->rect_ = Vector4(-0.05f, 0.05f, 0.6f, -0.05f);

#if _DEBUG && 1
  SetPos(Vector2::Random({0, 0}, {10, 10}),
         Vector2::Random(Vector2::X, Vector2::Y));
  SetColor(Vector3::Random(Vector3(0.6f, 0.6f, 0.6f), Vector3::ONE));
  SetLerpTime(Float::Random(0.07f, 0.1f));
  SetRadio(Float::Random(0.3f, 0.4f));
  web_delay_time_ = Float::Random(0.03f, 0.1f);
#endif  // Test
}
void TankComponent::OnUpdate() {
  though_t_ += 听君语::Get().GetDeltaTime();
  auto t = though_t_.ScaleFromTo(0, lerp_time_, 0, 1);
  GetXnObject()->pos_ = Vector2::Lerp(start_pos_, target_pos_, t);
  GetXnObject()->rotation_ = Float::Lerp(start_rotation_, target_rotation_, t);
#if _DEBUG && 1
  if (though_t_ >= web_delay_time_) {
    auto new_target_pos_ =
        target_pos_ + Vector2::Random({-0.5f, -0.5f}, {0.5f, 0.5f});
    new_target_pos_ = Vector2(Float::Clamp(new_target_pos_.x, 0, 10),
                              Float::Clamp(new_target_pos_.y, 0, 10));
    SetTargetPos(new_target_pos_, Vector2::Random(Vector2::X, Vector2::Y));
  }
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
  start_pos_ = GetXnObject()->pos_;
  start_rotation_ = GetXnObject()->rotation_;
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
  SetColor(Vector4(color, 1));
}
void TankComponent::SetColor(const Vector4 &color) {
  render_component_->color_ = color;
}

void TankComponent::BindUser() {
  // TODO 绑定用户信息
}
