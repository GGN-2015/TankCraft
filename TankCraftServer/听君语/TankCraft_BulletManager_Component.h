#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include <map>

#include "Component.h"
#include "Math.h"
#include "typedef.h"

namespace Xn {

namespace TankCraft {

class MapManagerComponent;
class BulletComponent;

// ��x��Vector2::X��Ϊǰ����
class BulletManagerComponent : public Component {
  friend class MapManagerComponent;

 public:
  BulletManagerComponent() : Component(L"TanksFight_BulletManagerComponent") {}

  virtual void OnStart() override;
  virtual void OnDestory() override;

 private:
  void StartSyncBulletState();
  void SetBulletState(const uint &bullet_id, const Vector2 &pos);
  void EndSyncBulletState();

  std::map<Int, BulletComponent *> unsynced_bullets;
  std::map<Int, BulletComponent *> synced_bullets;
};

}  // namespace TankCraft

}  // namespace Xn
