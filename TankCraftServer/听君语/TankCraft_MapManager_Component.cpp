#include "TankCraft_MapManager_Component.h"
//*********************************************************
//
// �ɡ����ѡ���д
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
#include "������.h"

void Xn::TankCraft::MapManagerComponent::OnStart() {
  render_component_ =
      (Square_RenderComponent*)GetXnObject()->SetRenderComponent(
          std::make_unique<Square_RenderComponent>(
              Vector4(0.8f, 1.f, 0.85f, 0.8f)));

  render_component_->rect_ = {-0.3f, -0.3f, 1 + 0.3f, 1 + 0.3f};

  wall_manager = (WallManagerComponent*)������::Get()
                     .GetObjectManager()
                     ->CreateXnObject(Vector2::ZERO, GetXnObject())
                     ->AddComponent(std::make_unique<WallManagerComponent>());
  tank_manager = (TankManagerComponent*)������::Get()
                     .GetObjectManager()
                     ->CreateXnObject(Vector2::ZERO, GetXnObject())
                     ->AddComponent(std::make_unique<TankManagerComponent>());

  // TODO Ŀǰ����֧�ֵ�ͼ���¶��룬���Ҳ���
  {
    Float width = ������::Get().GetRenderManager()->GetWidth();
    Float height = ������::Get().GetRenderManager()->GetHeight();

    auto small_map_pos = Vector2(width / 2.0f - height / 4.0f, height / 4.0f);
    auto small_map_scale = height / 2.0f;

    auto map_pos = Vector2(width / 2.0f - height / 2.0f, 0);
    auto map_scale = height;

    SetPos(small_map_pos, small_map_scale);
    SetTargetPos(map_pos, map_scale);
  }

  for (uint i = 0; i < 50; ++i) {
    ������::Get()
        .GetObjectManager()
        ->CreateXnObject(Vector2::ZERO, GetXnObject())
        ->AddComponent(std::make_unique<BulletComponent>());
  }

  for (uint i = 0; i < 5; ++i) {
    ������::Get()
        .GetObjectManager()
        ->CreateXnObject(Vector2::ZERO, GetXnObject())
        ->AddComponent(std::make_unique<TankComponent>());
  }

  // TODO �����ͼ8*8��ģ��һ��ǽ
  wchar* the_wall_data = new wchar[10];
  the_wall_data[0] = 49147;
  the_wall_data[1] = 21513;
  the_wall_data[2] = 17579;
  the_wall_data[3] = 4645;
  the_wall_data[4] = 34919;
  the_wall_data[5] = 4233;
  the_wall_data[6] = 38979;
  the_wall_data[7] = 40977;
  SetMap(the_wall_data, 8, 8);
  delete[] the_wall_data;
}
void Xn::TankCraft::MapManagerComponent::OnUpdate() {
  GetXnObject()->pos_ = Vector2::Lerp(GetXnObject()->pos_, target_pos_, 0.05f);
  GetXnObject()->scale_ =
      Vector2::Lerp(GetXnObject()->scale_, target_scale_, 0.05f);

  Float width = ������::Get().GetRenderManager()->GetWidth();
  Float height = ������::Get().GetRenderManager()->GetHeight();
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
