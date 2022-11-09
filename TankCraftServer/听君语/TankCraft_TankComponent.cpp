#include "TankCraft_TankComponent.h"
//*********************************************************
//
// ÓÉ¡¸ÂíÕÑ¡¹±àÐ´
//
//*********************************************************

#include "Circular_RenderComponent.h"
#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "Square_RenderComponent.h"
#include "TankCraft_UserManager_Component.h"
#include "Ìý¾ýÓï.h"

using namespace Xn;
using namespace TankCraft;

#define IS_TANK_LERP_MOVE 0

void TankComponent::OnStart() {
  render_component_ =
      (Circular_RenderComponent *)GetXnObject()->SetRenderComponent(
          std::make_unique<Circular_RenderComponent>());
  render_component_->color_ =
      Vector4(Vector3(200 / 255.f, 212 / 255.f, 230 / 255.f), 1.f);

  though_t_ = 0;

  // ÉèÖÃ°ë¾¶
  SetRadio(0.3f);

  SetLerpTime(0.1f);

  // ÉèÖÃÅÚ¹ÜÎ»ÖÃ
  gun_barrel_render_component_ =
      (Square_RenderComponent *)Ìý¾ýÓï::Get()
          .GetObjectManager()
          ->CreateXnObject(Vector2::ZERO, GetXnObject())
          ->SetRenderComponent(std::make_unique<Square_RenderComponent>(
              Vector4(162.f / 255.f, 168.f / 255.f, 160.f / 255.f, 1)));
  gun_barrel_render_component_->rect_ = Vector4(-0.05f, 0.05f, 0.23f, -0.05f);

#if _DEBUG && 0
  SetPos(Vector2::Random({0, 0}, {10, 10}), 0.f);
  web_delay_time_ = Float::Random(0.03f, 0.1f);
#endif  // Test
}
void TankComponent::OnUpdate() {
#if IS_TANK_LERP_MOVE
  though_t_ += Ìý¾ýÓï::Get().GetDeltaTime();
  auto t = though_t_.ScaleFromTo(0, lerp_time_, 0, 1);
  GetXnObject()->pos_ = Vector2::Lerp(start_pos_, target_pos_, t);
  GetXnObject()->rotation_ = Float::Lerp(start_rotation_, target_rotation_, t);
#endif  // IS_TANK_LERP_MOVE

#if _DEBUG && 0
  if (though_t_ >= web_delay_time_) {
    auto new_target_pos_ =
        target_pos_ + Vector2::Random({-0.5f, -0.5f}, {0.5f, 0.5f});
    new_target_pos_ = Vector2(Float::Clamp(new_target_pos_.x, 0, 10),
                              Float::Clamp(new_target_pos_.y, 0, 10));
    SetTargetPos(new_target_pos_, 45);
  }
#endif  // Test

  if (user_data_) {
    render_component_->color_ = user_data_->color;
  }
}
void TankComponent::OnDestory() {}

void TankComponent::SetLerpTime(const Float &lerp_time) {
  lerp_time_ = lerp_time;
}
void TankComponent::SetPos(const Vector2 &pos, const Float &rotation) {
#if IS_TANK_LERP_MOVE
  GetXnObject()->pos_ = start_pos_ = target_pos_ = pos;
  GetXnObject()->rotation_ = start_rotation_ = target_rotation_ = rotation;
#else
  GetXnObject()->pos_ = pos;
  GetXnObject()->rotation_ = rotation;
#endif  // IS_TANK_LERP_MOVE
}
void TankComponent::SetTargetPos(const Vector2 &pos, const Float &rotation) {
#if IS_TANK_LERP_MOVE
  though_t_ = 0;
  start_pos_ = GetXnObject()->pos_;
  start_rotation_ = GetXnObject()->rotation_;
  target_pos_ = pos;
  target_rotation_ = rotation;
#else
  GetXnObject()->pos_ = pos;
  GetXnObject()->rotation_ = rotation;
#endif  // IS_TANK_LERP_MOVE
}
void TankComponent::SetRadio(const Float &radius) {
  render_component_->radius_ = radius;
}

void TankComponent::BindUser(const UserData *const &user_data) {
  user_data_ = user_data;
}
