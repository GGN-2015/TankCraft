#include "TankCraft_WallManager_Component.h"

#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "TankCraft_WallComponent.h"
#include "Ìý¾ýÓï.h"

void Xn::TankCraft::WallManagerComponent::OnStart() {}

void Xn::TankCraft::WallManagerComponent::SetMap(const wchar *map_data,
                                                 const uint &x_side_length,
                                                 const uint &y_side_length) {
  x_side_length_ = x_side_length;
  y_side_length_ = y_side_length;

  const uint one_line_wchar_num = 2 * x_side_length / (8 * sizeof(wchar));

  GetXnObject()->RemoveAllChild();

  for (uint i = 0; i < y_side_length; ++i) {
    for (uint j = 0; j < one_line_wchar_num; ++j) {
      wchar c = map_data[i * one_line_wchar_num + j];

      wchar bit = 1;
      for (uint k = 0; k < 8; ++k) {
        const uint start_point = 8 * j + k;
        if (c & bit) AddWall({start_point, i}, {start_point, i + 1});
        bit <<= 1;

        if (c & bit) AddWall({start_point, i}, {start_point + 1, i});
        bit <<= 1;
      }
    }
  }

  for (uint i = 0; i < y_side_length; ++i) {
    AddWall({x_side_length, i}, {x_side_length, i + 1});
  }
  for (uint i = 0; i < x_side_length; ++i) {
    AddWall({i, y_side_length}, {i + 1, y_side_length});
  }
}

void Xn::TankCraft::WallManagerComponent::AddWall(const Vector2 &start_point,
                                                  const Vector2 &end_point) {
  auto wall = (WallComponent *)Ìý¾ýÓï::Get()
                  .GetObjectManager()
                  ->CreateXnObject(Vector2::ZERO, GetXnObject())
                  ->AddComponent(std::make_unique<WallComponent>());
  wall->SetPos(start_point, end_point);
  wall->SetWidth(0.1f);
  wall->SetColor({1, 0.6f, 0.7f, 1});
}
