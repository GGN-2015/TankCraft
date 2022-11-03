#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include "Component.h"
#include "Math.h"
#include "typedef.h"

namespace Xn {

class Transform_RenderComponent;
class Square_RenderComponent;

namespace TankCraft {

class WallManagerComponent;

// ��x��Vector2::X��Ϊǰ����
class MapComponent : public Component {
 public:
  MapComponent() : Component(L"TanksFight_MapComponent") {}

  virtual void OnStart();
  virtual void OnUpdate();
  virtual void OnDestory();

  void SetPos(const Vector2 &pos, const Float &scale);
  void SetTargetPos(const Vector2 &pos, const Float &scale);

 private:
  void SetMap(const wchar *const &map_data, const uint &x_side_length,
              const uint &y_side_length);

 private:
  uint x_side_length_ = 1;
  uint y_side_length_ = 1;

  WallManagerComponent *wall_manager = nullptr;

 private:
  Vector2 target_pos_;
  Vector2 target_scale_;

  Square_RenderComponent *render_component_ = nullptr;
};

}  // namespace TankCraft

}  // namespace Xn
