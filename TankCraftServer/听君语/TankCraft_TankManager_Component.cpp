#include "TankCraft_TankManager_Component.h"

#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "TankCraft_TankComponent.h"
#include "TankCraft_UserData.h"
#include "听君语.h"

constexpr int tank_state_justmalloc = 77;
constexpr int tank_state_stop = 94;

void Xn::TankCraft::TankManagerComponent::OnStart() {}

void Xn::TankCraft::TankManagerComponent::StartSyncTankState() {
  unsynced_tanks = std::move(synced_tanks);
}
void Xn::TankCraft::TankManagerComponent::SetTankState(const Int& user_id,
                                                       const Vector2& pos,
                                                       const Float& rotation,
                                                       const Int& state) {
  // 看看坦克有没有
  auto tank = unsynced_tanks.find(user_id);
  TankComponent* new_tank;
  if (tank == unsynced_tanks.end()) {
    // 没有userid坦克，就造一个新坦克
    new_tank = (TankComponent*)听君语::Get()
                   .GetObjectManager()
                   ->CreateXnObject(Vector2::ZERO, GetXnObject())
                   ->AddComponent(std::make_unique<TankComponent>());
    new_tank->SetPos(pos, rotation);
  } else {
    // 有，就从未同步列表删去
    new_tank = tank->second;
    unsynced_tanks.erase(tank);
    if (new_tank->state_ == tank_state_justmalloc ||
        new_tank->state_ == tank_state_stop) {
      // 未初始化坦克，初始化一下
      new_tank->GetXnObject()->SetActive(true);
      new_tank->SetPos(pos, rotation);
    } else
      // 正常运行的坦克，继续走吧
      new_tank->SetTargetPos(pos, rotation);
  }
  // 插入到已同步列表中
  new_tank->SetState(state);
  synced_tanks.insert({user_id, new_tank});
}
void Xn::TankCraft::TankManagerComponent::EndSyncTankState() {
  // 将没有的坦克全都将状态设为【停止运行】
  for (auto& item : unsynced_tanks) {
    item.second->state_ = tank_state_stop;
    item.second->GetXnObject()->SetActive(false);
    synced_tanks.insert({item.first, item.second});
  }
  unsynced_tanks.clear();
}
void Xn::TankCraft::TankManagerComponent::TryBindUser(
    const UserData* const& user_data) {
  // 看看坦克有没有
  auto tank = synced_tanks.find(user_data->user_id);
  if (tank != synced_tanks.end()) {
    // 有：直接绑定
    tank->second->BindUser(user_data);
  } else {
    // 没有：创建并绑定，将坦克状态设置为【仅申请空间】
    TankComponent* new_tank =
        (TankComponent*)听君语::Get()
            .GetObjectManager()
            ->CreateXnObject(Vector2::ZERO, GetXnObject())
            ->AddComponent(std::make_unique<TankComponent>());
    synced_tanks.insert({user_data->user_id, new_tank});
    new_tank->user_data_ = user_data;
    new_tank->GetXnObject()->SetActive(false);
  }
}
