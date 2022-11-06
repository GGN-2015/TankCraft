#include "TankCraft_MapManager_Component.h"
//*********************************************************
//
// ÓÉ¡¸ÂíÕÑ¡¹±àÐ´
//
//*********************************************************

#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "RenderManager.h"
#include "Square_RenderComponent.h"
#include "TankCraft_BulletComponent.h"
#include "TankCraft_TankComponent.h"
#include "TankCraft_TankManager_Component.h"
#include "TankCraft_WallManager_Component.h"
#include "Transform_RenderComponent.h"
#include "Ìý¾ýÓï.h"

void Xn::TankCraft::MapManagerComponent::OnStart() {
  render_component_ =
      (Square_RenderComponent*)GetXnObject()->SetRenderComponent(
          std::make_unique<Square_RenderComponent>(
              Vector4(0.8f, 1.f, 0.85f, 0.8f)));

  render_component_->rect_ = {-0.3f, -0.3f, 1 + 0.3f, 1 + 0.3f};

  wall_manager = (WallManagerComponent*)Ìý¾ýÓï::Get()
                     .GetObjectManager()
                     ->CreateXnObject(Vector2::ZERO, GetXnObject())
                     ->AddComponent(std::make_unique<WallManagerComponent>());
  tank_manager = (TankManagerComponent*)Ìý¾ýÓï::Get()
                     .GetObjectManager()
                     ->CreateXnObject(Vector2::ZERO, GetXnObject())
                     ->AddComponent(std::make_unique<TankManagerComponent>());

  // TODO Ä¿Ç°£¬½öÖ§³ÖµØÍ¼ÉÏÏÂ¶ÔÆë£¬×óÓÒ²»¹Ü
  {
    Float width = Ìý¾ýÓï::Get().GetRenderManager()->GetWidth();
    Float height = Ìý¾ýÓï::Get().GetRenderManager()->GetHeight();

    auto small_map_pos = Vector2(width / 2.0f - height / 4.0f, height / 4.0f);
    auto small_map_scale = height / 2.0f;

    auto map_pos = Vector2(width / 2.0f - height / 2.0f, 0);
    auto map_scale = height;

    SetPos(small_map_pos, small_map_scale);
    SetTargetPos(map_pos, map_scale);
  }
}
void Xn::TankCraft::MapManagerComponent::OnUpdate() {
  GetXnObject()->pos_ = Vector2::Lerp(GetXnObject()->pos_, target_pos_, 0.05f);
  GetXnObject()->scale_ =
      Vector2::Lerp(GetXnObject()->scale_, target_scale_, 0.05f);

  Float width = Ìý¾ýÓï::Get().GetRenderManager()->GetWidth();
  Float height = Ìý¾ýÓï::Get().GetRenderManager()->GetHeight();
  auto map_pos = Vector2(width / 2.0f - height / 2.0f, 0);
  auto map_scale = height / Float(y_side_length_);

  auto amend_scale_div = map_scale * 0.1f;
  map_pos.x += amend_scale_div * Float(x_side_length_) / 2.0f;
  map_pos.y += amend_scale_div * Float(y_side_length_) / 2.0f;
  map_scale -= amend_scale_div;
  SetTargetPos(map_pos, map_scale);
}
void Xn::TankCraft::MapManagerComponent::OnDestory() {}

void Xn::TankCraft::MapManagerComponent::SetPos(const Vector2& pos,
                                                const Float& scale) {
  GetXnObject()->pos_ = target_pos_ = pos;
  GetXnObject()->scale_ = target_scale_ = Vector2(scale, scale);
}
void Xn::TankCraft::MapManagerComponent::SetTargetPos(const Vector2& pos,
                                                      const Float& scale) {
  target_pos_ = pos;
  target_scale_ = Vector2(scale, scale);
}

void Xn::TankCraft::MapManagerComponent::SetMap(const wchar* const& map_data,
                                                const uint& x_side_length,
                                                const uint& y_side_length) {
  x_side_length_ = x_side_length;
  y_side_length_ = y_side_length;
  render_component_->rect_ = {-0.3f, -0.3f, x_side_length_ + 0.3f,
                              y_side_length_ + 0.3f};
  wall_manager->SetMap(map_data, x_side_length, y_side_length);
}

void Xn::TankCraft::MapManagerComponent::StartSyncTankState() {
  tank_manager->StartSyncTankState();
}

void Xn::TankCraft::MapManagerComponent::SetTankState(const Int& usesr_id,
                                                      const Vector2& pos,
                                                      const Float& rotation,
                                                      const Int& state) {
  tank_manager->SetTankState(usesr_id, pos, rotation, state);
}

void Xn::TankCraft::MapManagerComponent::EndSyncTankState() {
  tank_manager->EndSyncTankState();
}

void Xn::TankCraft::MapManagerComponent::TryBindUser(
    const UserData* const& user_data) {
  tank_manager->TryBindUser(user_data);
}