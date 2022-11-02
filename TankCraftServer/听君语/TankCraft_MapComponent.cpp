#include "TankCraft_MapComponent.h"
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "RenderManager.h"
#include "Square_RenderComponent.h"
#include "TankCraft_TankComponent.h"
#include "TankCraft_WallComponent.h"
#include "Transform_RenderComponent.h"
#include "������.h"

// TODO Ŀǰд�������̴�С
constexpr uint map_side_length = 10;

void Xn::TankCraft::MapComponent::OnStart() {
  render_component_ =
      (Square_RenderComponent*)GetXnObject()->SetRenderComponent(
          std::make_unique<Square_RenderComponent>(Vector4(1, 0, 0, 1)));

  render_component_->rect_ = {0, 0, 100, 100};

  // TODO Ŀǰ����֧�ֵ�ͼ���¶��룬���Ҳ���
  {
    Float width = ������::Get().GetRenderManager()->GetWidth();
    Float height = ������::Get().GetRenderManager()->GetHeight();

    auto small_map_pos =
        Vector2(width / Float(2) - height * Float(1) / Float(4),
                height * Float(1) / Float(4));
    auto small_map_scale = (height / (Float)map_side_length) / Float(2);

    auto map_pos = Vector2(width / Float(2) - height / Float(2), 0);
    auto map_scale = height / Float(map_side_length);

    SetPos(small_map_pos, small_map_scale);
    SetTargetPos(map_pos, map_scale);
  }

  for (uint i = 0; i < 5; ++i) {
    ������::Get()
        .GetObjectManager()
        ->CreateXnObject(Vector2::ZERO, GetXnObject())
        ->AddComponent(std::make_unique<TankComponent>());
  }

  // TODO �����ͼ10*10��ģ��һ��ǽ
  for (uint i = 0; i < 10; ++i) {
    for (uint j = 0; j < 10; ++j) {
      auto wall = (WallComponent*)������::Get()
                      .GetObjectManager()
                      ->CreateXnObject(Vector2::ZERO, GetXnObject())
                      ->AddComponent(std::make_unique<WallComponent>());
      wall->SetPos({j, i},
                   (rand() & 1) ? Vector2(j + 1, i) : Vector2(j, i + 1));
      wall->SetWidth(0.1f);
      wall->SetColor(Vector4(1, 0.6f, 0.7f, 1));
    }
  }
}
void Xn::TankCraft::MapComponent::OnUpdate() {
  GetXnObject()->pos_ = Vector2::Lerp(GetXnObject()->pos_, target_pos_, 0.05f);
  GetXnObject()->scale_ =
      Vector2::Lerp(GetXnObject()->scale_, target_scale_, 0.05f);

  Float width = ������::Get().GetRenderManager()->GetWidth();
  Float height = ������::Get().GetRenderManager()->GetHeight();
  auto map_pos = Vector2(width / Float(2) - height / Float(2), 0);
  auto map_scale = height / Float(map_side_length);
  SetTargetPos(map_pos, map_scale);
}
void Xn::TankCraft::MapComponent::OnDestory() {}

void Xn::TankCraft::MapComponent::SetPos(const Vector2& pos,
                                         const Float& scale) {
  GetXnObject()->pos_ = target_pos_ = pos;
  GetXnObject()->scale_ = target_scale_ = Vector2(scale, scale);
}
void Xn::TankCraft::MapComponent::SetTargetPos(const Vector2& pos,
                                               const Float& scale) {
  target_pos_ = pos;
  target_scale_ = Vector2(scale, scale);
}
