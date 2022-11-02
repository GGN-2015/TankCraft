#include "TankCraft_WallComponent.h"
//*********************************************************
//
// ”…°∏¬Ì’—°π±‡–¥
//
//*********************************************************

#include <string>

#include "Line_RenderComponent.h"
#include "ObjectManager-XnObject.h"
#include "stdafx.h"

void Xn::TankCraft::WallComponent::OnStart() {
  render_component_ = (Line_RenderComponent*)GetXnObject()->SetRenderComponent(
      std::make_unique<Line_RenderComponent>());
}

void Xn::TankCraft::WallComponent::SetPos(const Vector2& start_point,
                                          const Vector2& end_point) {
  start_point_ = start_point;
  end_point_ = end_point;
  AmendRenderPoints();
  OutputDebugStringW((L"WallPos is (" + std::to_wstring((float)start_point.x) +
                      L", " + std::to_wstring((float)start_point.y) + L")!\n")
                         .c_str());
}

void Xn::TankCraft::WallComponent::SetWidth(const Float& width) {
  render_component_->width_ = width;
  AmendRenderPoints();
}

void Xn::TankCraft::WallComponent::SetColor(const Vector3& color) {
  SetColor({color.x, color.y, color.z, Float::ONE});
}

void Xn::TankCraft::WallComponent::SetColor(const Vector4& color) {
  render_component_->color_ = color;
}

void Xn::TankCraft::WallComponent::AmendRenderPoints() {
  auto direct = end_point_ - start_point_;
  auto amend_value = Vector2(direct.x * render_component_->width_ / Float(2),
                             direct.y * render_component_->width_ / Float(2));

  render_component_->start_point_ = start_point_ - amend_value;
  render_component_->end_point_ = end_point_ + amend_value;
}
