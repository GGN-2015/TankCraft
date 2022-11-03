#include "TankCraft_BulletComponent.h"
//*********************************************************
//
// ÓÉ¡¸ÂíÕÑ¡¹±àÐ´
//
//*********************************************************

#include "Circular_RenderComponent.h"
#include "ObjectManager-XnObject.h"
#include "Ìý¾ýÓï.h"

void Xn::TankCraft::BulletComponent::OnStart() {
  render_component_ =
      (Circular_RenderComponent*)GetXnObject()->SetRenderComponent(
          std::make_unique<Circular_RenderComponent>());

  though_t_ = 0;

#if _DEBUG && 1
  SetPos(Vector2::Random({0, 0}, {10, 10}));
  SetColor(Vector3::Random(Vector3::ZERO, Vector3(0.3f, 0.3f, 0.3f)));
  SetLerpTime(Float::Random(0.07f, 0.1f));
  SetRadio(0.04f);
  web_delay_time_ = Float::Random(0.03f, 0.1f);
#endif  // Test
}
void Xn::TankCraft::BulletComponent::OnUpdate() {
  though_t_ += Ìý¾ýÓï::Get().GetDeltaTime();
  auto t = though_t_.ScaleFromTo(0, lerp_time_, 0, 1);
  GetXnObject()->pos_ = Vector2::Lerp(start_pos_, target_pos_, t);
#if _DEBUG && 1
  if (though_t_ >= web_delay_time_) {
    auto new_target_pos_ =
        target_pos_ + Vector2::Random({-0.5f, -0.5f}, {0.5f, 0.5f});
    new_target_pos_ = Vector2(Float::Clamp(new_target_pos_.x, 0, 10),
                              Float::Clamp(new_target_pos_.y, 0, 10));
    SetTargetPos(new_target_pos_);
  }
#endif  // Test
}

void Xn::TankCraft::BulletComponent::SetLerpTime(const Float& lerp_time) {
  lerp_time_ = lerp_time;
}
void Xn::TankCraft::BulletComponent::SetPos(const Vector2& pos) {
  GetXnObject()->pos_ = start_pos_ = target_pos_ = pos;
}
void Xn::TankCraft::BulletComponent::SetTargetPos(const Vector2& pos) {
  though_t_ = 0;
  start_pos_ = GetXnObject()->pos_;
  target_pos_ = pos;
}
void Xn::TankCraft::BulletComponent::SetRadio(const Float& radius) {
  render_component_->radius_ = radius;
}
void Xn::TankCraft::BulletComponent::SetColor(const Vector3& color) {
  SetColor(Vector4(color, 1));
}
void Xn::TankCraft::BulletComponent::SetColor(const Vector4& color) {
  render_component_->color_ = color;
}
