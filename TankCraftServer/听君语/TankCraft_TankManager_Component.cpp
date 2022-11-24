#include "TankCraft_TankManager_Component.h"

#include "Circular_RenderComponent.h"
#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "TankCraft_TankComponent.h"
#include "TankCraft_UserData.h"
#include "TankCraft_UserManager_Component.h"
#include "������.h"

constexpr int tank_state_justmalloc = 77;
constexpr int tank_state_stop = 94;

void Xn::TankCraft::TankManagerComponent::OnStart() {}

void Xn::TankCraft::TankManagerComponent::OnUpdate() {
  for (auto item : synced_tanks) {
    auto user = user_manager_->GetUser(item.first);
    if (!user) continue;

    item.second->tank_body_render_component_->color_ = user->color.asColor;
  }
}

void Xn::TankCraft::TankManagerComponent::StartSyncTankState() {
  unsynced_tanks = std::move(synced_tanks);
}
void Xn::TankCraft::TankManagerComponent::SetTankState(const Int& user_id,
                                                       const Vector2& pos,
                                                       const Float& rotation,
                                                       const Int& state) {
  // ����̹����û��
  auto tank = unsynced_tanks.find(user_id);
  TankComponent* new_tank;
  if (tank == unsynced_tanks.end()) {
    // û��userid̹�ˣ�����һ����̹��
    new_tank = (TankComponent*)������::Get()
                   .GetObjectManager()
                   ->CreateXnObject(Vector2::ZERO, GetXnObject())
                   ->AddComponent(std::make_unique<TankComponent>());
    new_tank->SetPos(pos, rotation);
  } else {
    // �У��ʹ�δͬ���б�ɾȥ
    new_tank = tank->second;
    unsynced_tanks.erase(tank);
    if (new_tank->state_ == tank_state_justmalloc ||
        new_tank->state_ == tank_state_stop) {
      // δ��ʼ��̹�ˣ���ʼ��һ��
      new_tank->GetXnObject()->SetActive(true);
      new_tank->SetPos(pos, rotation);
    } else
      // �������е�̹�ˣ������߰�
      new_tank->SetTargetPos(pos, rotation);
  }
  // ���뵽��ͬ���б���
  new_tank->SetState(state);
  synced_tanks.insert({user_id, new_tank});
}
void Xn::TankCraft::TankManagerComponent::EndSyncTankState() {
  // ��û�е�̹��ȫ����״̬��Ϊ��ֹͣ���С�
  for (auto& item : unsynced_tanks) {
    item.second->state_ = tank_state_stop;
    item.second->GetXnObject()->SetActive(false);
    synced_tanks.insert({item.first, item.second});
  }
  unsynced_tanks.clear();
}
void Xn::TankCraft::TankManagerComponent::TryBindUser(const uint& user_id) {
  // ����̹����û��
  auto tank = synced_tanks.find(user_id);
  if (tank != synced_tanks.end()) {
    // �У�ֱ�Ӱ�
    tank->second->BindUser(user_id);
  } else {
    // û�У��������󶨣���̹��״̬����Ϊ��������ռ䡿
    TankComponent* new_tank =
        (TankComponent*)������::Get()
            .GetObjectManager()
            ->CreateXnObject(Vector2::ZERO, GetXnObject())
            ->AddComponent(std::make_unique<TankComponent>());
    synced_tanks.insert({user_id, new_tank});
    new_tank->user_id_ = user_id;
    new_tank->GetXnObject()->SetActive(false);
  }
}
