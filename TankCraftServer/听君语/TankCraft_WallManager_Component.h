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

namespace TankCraft {

class MapComponent;

// 以x（Vector2::X）为前方向
class WallManagerComponent : public Component {
  friend class MapComponent;

 public:
  WallManagerComponent() : Component(L"TanksFight_MapComponent") {}

  virtual void OnStart();

 private:
  void SetMap(const wchar *map_data, const uint &x_side_length,
              const uint &y_side_length);
  void AddWall(const Vector2 &start_point, const Vector2 &end_point);

 private:
  uint x_side_length_ = 1;
  uint y_side_length_ = 1;

  // TODO 目前换图是直接全删除，全添加，以后改为仅设置，不全删
};

}  // namespace TankCraft

}  // namespace Xn
