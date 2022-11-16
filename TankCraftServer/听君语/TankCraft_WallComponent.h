#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include "Component.h"
#include "Math.h"

namespace Xn {

class Line_RenderComponent;
class Circular_RenderComponent;

namespace TankCraft {

// ��x��Vector2::X��Ϊǰ����
class WallComponent : public Component {
 public:
  WallComponent() : Component(L"TanksFight_WallComponent") {}

  virtual void OnStart() override;

  void SetPos(const Vector2 &start_point, const Vector2 &end_point);
  void SetWidth(const Float &width);
  void SetColor(const Vector3 &color);
  void SetColor(const Vector4 &color);

 private:
  void AmendRenderPoints();

 private:
  Vector2 start_point_;
  Vector2 end_point_;

  Line_RenderComponent *render_component_ = nullptr;
  Circular_RenderComponent *ends_render_component_[2] = {};
};

}  // namespace TankCraft

}  // namespace Xn
