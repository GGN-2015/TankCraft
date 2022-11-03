#include "TankCraft_MapComponent.h"
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
#include "TankCraft_WallComponent.h"
#include "Transform_RenderComponent.h"
#include "Ìý¾ýÓï.h"

// TODO Ä¿Ç°Ð´ËÀµÄÆåÅÌ´óÐ¡
const Xn::Int map_side_length(10);

void Xn::TankCraft::MapComponent::OnStart() {
  render_component_ =
      (Square_RenderComponent*)GetXnObject()->SetRenderComponent(
          std::make_unique<Square_RenderComponent>(
              Vector4(0.7f, 1.f, 0.75f, 1)));

  render_component_->rect_ = {-0.3f, -0.3f, map_side_length.ToFloat() + 0.3f,
                              map_side_length.ToFloat() + 0.3f};

  // TODO Ä¿Ç°£¬½öÖ§³ÖµØÍ¼ÉÏÏÂ¶ÔÆë£¬×óÓÒ²»¹Ü
  {
    Float width = Ìý¾ýÓï::Get().GetRenderManager()->GetWidth();
    Float height = Ìý¾ýÓï::Get().GetRenderManager()->GetHeight();

    auto small_map_pos = Vector2(width / 2.0f - height / 4.0f, height / 4.0f);
    auto small_map_scale = (height / map_side_length.ToFloat()) / 2.0f;

    auto map_pos = Vector2(width / 2.0f - height / 2.0f, 0);
    auto map_scale = height / map_side_length.ToFloat();

    SetPos(small_map_pos, small_map_scale);
    SetTargetPos(map_pos, map_scale);
  }

  for (uint i = 0; i < 50; ++i) {
    Ìý¾ýÓï::Get()
        .GetObjectManager()
        ->CreateXnObject(Vector2::ZERO, GetXnObject())
        ->AddComponent(std::make_unique<BulletComponent>());
  }

  for (uint i = 0; i < 5; ++i) {
    Ìý¾ýÓï::Get()
        .GetObjectManager()
        ->CreateXnObject(Vector2::ZERO, GetXnObject())
        ->AddComponent(std::make_unique<TankComponent>());
  }

  // TODO ¼ÓÈëµØÍ¼10*10£¬Ä£ÄâÒ»ÏÂÇ½
}
void Xn::TankCraft::MapComponent::OnUpdate() {
  GetXnObject()->pos_ = Vector2::Lerp(GetXnObject()->pos_, target_pos_, 0.05f);
  GetXnObject()->scale_ =
      Vector2::Lerp(GetXnObject()->scale_, target_scale_, 0.05f);

  Float width = Ìý¾ýÓï::Get().GetRenderManager()->GetWidth();
  Float height = Ìý¾ýÓï::Get().GetRenderManager()->GetHeight();
  auto map_pos = Vector2(width / 2.0f - height / 2.0f, 0);
  auto map_scale = height / map_side_length.ToFloat();
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

void Xn::TankCraft::MapComponent::SetMap(const wchar* const& map_data,
                                         const uint& x_side_length,
                                         const uint& y_side_length) {
  for (uint i = 0; i < y_side_length; ++i) {
    for (uint j = 0; j < 2.0f * x_side_length / (8 * sizeof(wchar)); ++j) {
      union {};
      auto wall = (WallComponent*)Ìý¾ýÓï::Get()
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
