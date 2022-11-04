#include "TankCraft_TankManager_Component.h"

#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "TankCraft_TankComponent.h"
#include "Ìý¾ýÓï.h"

void Xn::TankCraft::TankManagerComponent::OnStart() {}

void Xn::TankCraft::TankManagerComponent::StartSyncTankState() {
  unsynced_tanks = std::move(synced_tanks);
}
void Xn::TankCraft::TankManagerComponent::SetTankState(const Int& usesr_id,
                                                       const Vector2& pos,
                                                       const Float& rotation,
                                                       const Int& state) {
  auto tank = unsynced_tanks.find(usesr_id);
  TankComponent* new_tank;
  if (tank == unsynced_tanks.end()) {
    new_tank = (TankComponent*)Ìý¾ýÓï::Get()
                   .GetObjectManager()
                   ->CreateXnObject(Vector2::ZERO, GetXnObject())
                   ->AddComponent(std::make_unique<TankComponent>());
    new_tank->SetPos(pos, rotation);
  } else {
    new_tank = tank->second;
    unsynced_tanks.erase(tank);
    new_tank->SetTargetPos(pos, rotation);
  }
  synced_tanks.insert({usesr_id, new_tank});
}
void Xn::TankCraft::TankManagerComponent::EndSyncTankState() {
  for (auto& item : unsynced_tanks) {
    GetXnObject()->RemoveChild(item.second->GetXnObject()->GetId());
  }
  unsynced_tanks.clear();
}
