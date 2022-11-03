#pragma once
//*********************************************************
//
// ÓÉ¡¸ÂíÕÑ¡¹±àÐ´
//
//*********************************************************

#include "Component.h"
#include "Math.h"
#include "ObjectManager-XnObject.h"
#include "RenderComponent.h"
#include "Square_RenderComponent.h"
#include "Ìý¾ýÓï.h"

namespace Xn {

class TestComponent2 : public Component {
 public:
  TestComponent2() : Component(L"TestComponent2") {}

  virtual void OnStart() override {
    render_component_ =
        (Square_RenderComponent*)GetXnObject()->SetRenderComponent(
            std::make_unique<Square_RenderComponent>(Vector4(1, 1, 1, 1)));

    one_move_t_ = 1.5f;
    though_t_ = 0;

    start_pos_ = GetXnObject()->GetPos();
    start_scale_ = GetXnObject()->GetScale();
    start_rotation_ = GetXnObject()->GetRotation();
    start_color_ = render_component_->color_;
    RollNewTarget();
  };
  virtual void OnUpdate() override {
    though_t_ += Ìý¾ýÓï::Get().GetDeltaTime();
    if (though_t_ >= one_move_t_) {
      though_t_ = 0;
      GetXnObject()->pos_ = start_pos_ = target_pos_;
      GetXnObject()->scale_ = start_scale_ = target_scale_;
      GetXnObject()->rotation_ = start_rotation_ = target_rotation_;
      render_component_->color_ = start_color_ = target_color_;
      RollNewTarget();
    } else {
      GetXnObject()->pos_ = Vector2::Lerp(start_pos_, target_pos_, though_t_);
      GetXnObject()->scale_ =
          Vector2::Lerp(start_scale_, target_scale_, though_t_);
      GetXnObject()->rotation_ =
          Float::Lerp(start_rotation_, target_rotation_, though_t_);
      render_component_->color_ =
          Vector4::Lerp(start_color_, target_color_, though_t_);
    }
  };
  virtual void OnDestory() override{};

  void RollNewTarget() {
    target_pos_ = Vector2(0, 0);
    // target_pos_ = Vector2::RandomBetween(Vector2(100, 100), Vector2(400,
    // 400));
    target_scale_ = Vector2::Random(Vector2(0.5f, 0.5f), Vector2(2, 2));
    target_rotation_ = Float::Random(-45, 45);
    target_color_ =
        Vector4::Random(Vector4(1, 0, 0, 1), Vector4(1, 1, 1, 1));
  }

 private:
  Square_RenderComponent* render_component_ = nullptr;

  Float one_move_t_ = 0;
  Float though_t_ = 0;

  Vector2 start_pos_;
  Vector2 start_scale_;
  Float start_rotation_ = 0;
  Vector4 start_color_;

  Vector2 target_pos_;
  Vector2 target_scale_;
  Float target_rotation_ = 0;
  Vector4 target_color_;
};

class TestComponent : public Component {
 public:
  TestComponent() : Component(L"TestComponent") {}

  virtual void OnStart() override {
    render_component_ =
        (Square_RenderComponent*)GetXnObject()->SetRenderComponent(
            std::make_unique<Square_RenderComponent>(Vector4(1, 1, 1, 1)));

    one_move_t_ = 2;
    though_t_ = 0;

    start_pos_ = GetXnObject()->GetPos();
    start_scale_ = GetXnObject()->GetScale();
    start_rotation_ = GetXnObject()->GetRotation();
    start_color_ = render_component_->color_;
    RollNewTarget();

    Ìý¾ýÓï::Get()
        .GetObjectManager()
        ->CreateXnObject(Vector2(0.f, 0.5f), GetXnObject())
        ->AddComponent(std::make_unique<TestComponent2>());
  };
  virtual void OnUpdate() override {
    though_t_ += Ìý¾ýÓï::Get().GetDeltaTime();
    if (though_t_ >= one_move_t_) {
      though_t_ = 0;
      GetXnObject()->pos_ = start_pos_ = target_pos_;
      GetXnObject()->scale_ = start_scale_ = target_scale_;
      GetXnObject()->rotation_ = start_rotation_ = target_rotation_;
      render_component_->color_ = start_color_ = target_color_;
      RollNewTarget();
    } else {
      GetXnObject()->pos_ = Vector2::Lerp(start_pos_, target_pos_, though_t_);
      GetXnObject()->scale_ =
          Vector2::Lerp(start_scale_, target_scale_, though_t_);
      GetXnObject()->rotation_ =
          Float::Lerp(start_rotation_, target_rotation_, though_t_);
      render_component_->color_ =
          Vector4::Lerp(start_color_, target_color_, though_t_);
    }
  };
  virtual void OnDestory() override{};

  void RollNewTarget() {
    target_pos_ = Vector2::Random(Vector2(100, 100), Vector2(400, 400));
    target_scale_ = Vector2::Random(Vector2(100, 100), Vector2(400, 400));
    target_rotation_ = Float::Random(-45, 135);
    target_color_ =
        Vector4::Random(Vector4(1.f, 0.f, 0.f, 0.5f), Vector4(1, 1, 1, 1));
  }

 private:
  Square_RenderComponent* render_component_ = nullptr;

  Float one_move_t_;
  Float though_t_;

  Vector2 start_pos_;
  Vector2 start_scale_;
  Float start_rotation_;
  Vector4 start_color_;

  Vector2 target_pos_;
  Vector2 target_scale_;
  Float target_rotation_;
  Vector4 target_color_;
};

}  // namespace Xn