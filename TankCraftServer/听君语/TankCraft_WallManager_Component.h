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

namespace TankCraft {

class MapComponent;

// ��x��Vector2::X��Ϊǰ����
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

  // TODO Ŀǰ��ͼ��ֱ��ȫɾ����ȫ��ӣ��Ժ��Ϊ�����ã���ȫɾ
};

}  // namespace TankCraft

}  // namespace Xn
