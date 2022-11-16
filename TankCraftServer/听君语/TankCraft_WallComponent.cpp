#include "TankCraft_WallComponent.h"
//*********************************************************
//
// ”…°∏¬Ì’—°π±‡–¥
//
//*********************************************************

#include <string>

#include "Circular_RenderComponent.h"
#include "Line_RenderComponent.h"
#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "stdafx.h"
#include "Ã˝æ˝”Ô.h"

void Xn::TankCraft::WallComponent::OnStart() {
  render_component_ = (Line_RenderComponent*)GetXnObject()->SetRenderComponent(
      std::make_unique<Line_RenderComponent>());

  ends_render_component_[0] =
      (Circular_RenderComponent*)Ã˝æ˝”Ô::Get()
          .GetObjectManager()
          ->CreateXnObject(Vector2::ZERO, GetXnObject())
          ->SetRenderComponent(std::make_unique<Circular_RenderComponent>());
  ends_render_component_[1] =
      (Circular_RenderComponent*)Ã˝æ˝”Ô::Get()
          .GetObjectManager()
          ->CreateXnObject(Vector2::ZERO, GetXnObject())
          ->SetRenderComponent(std::make_unique<Circular_RenderComponent>());
}

void Xn::TankCraft::WallComponent::SetPos(const Vector2& start_point,
                                          const Vector2& end_point) {
  start_point_ = start_point;
  end_point_ = end_point;
  AmendRenderPoints();
}

void Xn::TankCraft::WallComponent::SetWidth(const Float& width) {
  render_component_->width_ = width;
  ends_render_component_[0]->radius_ = width / 2.f;
  ends_render_component_[1]->radius_ = width / 2.f;
  AmendRenderPoints();
}

void Xn::TankCraft::WallComponent::SetColor(const Vector3& color) {
  SetColor({color.x, color.y, color.z, Float::ONE});
}

void Xn::TankCraft::WallComponent::SetColor(const Vector4& color) {
  render_component_->color_ = color;
  ends_render_component_[0]->color_ = color.asColor;
  ends_render_component_[1]->color_ = color.asColor;
}

void Xn::TankCraft::WallComponent::AmendRenderPoints() {
  // auto direct = end_point_ - start_point_;
  // auto amend_value = Vector2(direct.x * render_component_->width_ / Float(2),
  //                            direct.y * render_component_->width_ /
  //                            Float(2));
  // render_component_->start_point_ = start_point_ - amend_value;
  // render_component_->end_point_ = end_point_ + amend_value;

  render_component_->start_point_ = start_point_;
  render_component_->end_point_ = end_point_;
  ends_render_component_[0]->GetXnObject()->pos_ =
      render_component_->start_point_;
  ends_render_component_[1]->GetXnObject()->pos_ =
      render_component_->end_point_;
}
