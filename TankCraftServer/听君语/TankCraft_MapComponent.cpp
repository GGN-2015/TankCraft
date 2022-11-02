#include "TankCraft_MapComponent.h"
//*********************************************************
//
// ÓÉ¡¸ÂíÕÑ¡¹±àÐ´
//
//*********************************************************

#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "Square_RenderComponent.h"
#include "TankCraft_TankComponent.h"
#include "Transform_RenderComponent.h"
#include "Ìý¾ýÓï.h"

void Xn::TankCraft::MapComponent::OnStart() {
  render_component_ =
      (Square_RenderComponent*)GetXnObject()->SetRenderComponent(
          std::make_unique<Square_RenderComponent>(Vector4(1, 0, 0, 1)));

  render_component_->rect_ = {0, 0, 100, 100};

  though_t_ = 0;

  SetLerpTime(Float::Random(1, 3));
  SetPos(Vector2::Random({0, 0}, {100, 100}), Float::Random(30, 40));
  SetTargetPos(Vector2::Random({0, 0}, {100, 100}), Float::Random(30, 50));

  for (uint i = 0; i < 5; ++i) {
    Ìý¾ýÓï::Get()
        .GetObjectManager()
        ->CreateXnObject(Vector2(5, 5), GetXnObject())
        ->AddComponent(std::make_unique<TankComponent>());
  }
}
void Xn::TankCraft::MapComponent::OnUpdate() {
  though_t_ += Ìý¾ýÓï::Get().GetDeltaTime();
  GetXnObject()->pos_ = Vector2::Lerp(start_pos_, target_pos_, though_t_);
  GetXnObject()->scale_ = Vector2::Lerp(start_scale_, target_scale_, though_t_);
}
void Xn::TankCraft::MapComponent::OnDestory() {}

void Xn::TankCraft::MapComponent::SetLerpTime(const Float& lerp_time) {
  lerp_time_ = lerp_time;
}
void Xn::TankCraft::MapComponent::SetPos(const Vector2& pos,
                                         const Float& scale) {
  GetXnObject()->pos_ = start_pos_ = target_pos_ = pos;
  GetXnObject()->scale_ = start_scale_ = target_scale_ = Vector2(scale, scale);
}
void Xn::TankCraft::MapComponent::SetTargetPos(const Vector2& pos,
                                               const Float& scale) {
  target_pos_ = pos;
  target_scale_ = Vector2(scale, scale);
}
