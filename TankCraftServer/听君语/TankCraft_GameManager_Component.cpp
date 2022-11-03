#include "TankCraft_GameManager_Component.h"

#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "TankCraft_MapManager_Component.h"
#include "TankCraft_UserManager_Component.h"
#include "Ìý¾ýÓï.h"

void Xn::TankCraft::GameManagerComponent::OnStart() {
  map_manager = (MapManagerComponent*)GetXnObject()->AddComponent(
      std::make_unique<MapManagerComponent>());
  user_manager = (UserManagerComponent*)GetXnObject()->AddComponent(
      std::make_unique<UserManagerComponent>());
}
void Xn::TankCraft::GameManagerComponent::OnUpdate() {}
void Xn::TankCraft::GameManagerComponent::OnDestory() {
  GetXnObject()->RemoveAllChild();
}

void Xn::TankCraft::GameManagerComponent::SetMap(const wchar* const& map_data,
                                                 const uint& x_side_length,
                                                 const uint& y_side_length) {
  map_manager->SetMap(map_data, x_side_length, y_side_length);
}
void Xn::TankCraft::GameManagerComponent::AddUsers(
    const wchar* const& users_data, const uint& user_count) {}
void Xn::TankCraft::GameManagerComponent::SetTanksState(
    const wchar* const& tanks_data, const uint& tank_count) {}
void Xn::TankCraft::GameManagerComponent::SetUsersKillNumber(
    const wchar* const& users_kill_number_data,
    const uint& users_kill_number_count) {}
void Xn::TankCraft::GameManagerComponent::SetEntitiesState(
    const wchar* const& entities_data, const uint& entity_count) {}

void Xn::TankCraft::GameManagerComponent::AddUser(
    const uint user_id, const uint& user_name_length,
    const wchar* const& user_name, const Vector4& user_color,
    const uint& user_kill_number) {}
void Xn::TankCraft::GameManagerComponent::SetBulletState(const uint& id,
                                                         const Vector2& pos) {}
void Xn::TankCraft::GameManagerComponent::SetTankBomb(const uint& user_id,
                                                      const Vector2& pos) {}
void Xn::TankCraft::GameManagerComponent::SetLaser(
    const wchar* const& laser_data, const uint& point_count) {}
