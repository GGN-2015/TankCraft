#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include "Component.h"
#include "Math.h"
#include "typedef.h"

namespace Xn {

class Square_RenderComponent;

namespace TankCraft {

// 以x（Vector2::X）为前方向
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
  uint x_side_length_;
  uint y_side_length_;

 private:
  Vector2 target_pos_;
  Vector2 target_scale_;

  // TODO 目前换图是直接全删除，全添加，以后改为仅设置，不全删
};

}  // namespace TankCraft

}  // namespace Xn
