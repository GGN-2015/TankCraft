#include "TankCraft_GameManager_Component.h"

#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "TankCraft_MapManager_Component.h"
#include "TankCraft_UserManager_Component.h"
#include "听君语.h"

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

void Xn::TankCraft::GameManagerComponent::InternalCommunicationMessage(
    const uint& code) {
  switch (code) {
    case 0: {
      // TODO 与服务端连接成功
    } break;
    case 1: {
      // TODO WAS启动时出错
    } break;
    case 2: {
      // TODO 套接字对象建立时出错
    } break;
    case 3: {
      // TODO 连接不到服务器
    } break;
    case 4: {
      // TODO 已连接到服务器，但是断开了连接
    } break;

    default: {
      // 未知消息
    } break;
  }
}
void Xn::TankCraft::GameManagerComponent::SetMap(const wchar* const& map_data,
                                                 const uint& x_side_length,
                                                 const uint& y_side_length) {
  map_manager->SetMap(map_data, x_side_length, y_side_length);
}
void Xn::TankCraft::GameManagerComponent::AddUsers(
    const wchar* const& users_data, const uint& user_count) {
  uint users_data_index = 0;

  for (uint add_user_index = 0; add_user_index < user_count; ++add_user_index) {
    const auto user_id = *(uint*)&users_data[users_data_index];
    users_data_index += 2;

    const auto user_name_length = *(uint16*)&users_data[users_data_index];
    users_data_index += 1;

    const auto user_name = &users_data[users_data_index];
    users_data_index += user_name_length;

    union RGBA {
      struct {
        uint8 r;
        uint8 g;
        uint8 b;
        uint8 a;
      };
      char data[4];
    } the_color = *(RGBA*)&users_data[users_data_index];
    const Vector4 user_color(the_color.r / 255.f, the_color.g / 255.f,
                             the_color.b / 255.f, the_color.a / 255.f);
    users_data_index += 2;

    const auto user_kill_number = *(uint16*)&users_data[users_data_index];
    users_data_index += 1;

    AddUser(user_id, user_name_length, user_name, user_color, user_kill_number);
  }
}
void Xn::TankCraft::GameManagerComponent::SetTanksState(
    const wchar* const& tanks_data, const uint& tank_count) {
  uint tank_data_index = 0;

  map_manager->StartSyncTankState();
  for (uint tank_index = 0; tank_index < tank_count; ++tank_index) {
    const auto user_id = *(uint*)&tanks_data[tank_data_index];
    tank_data_index += 2;

    union Pos {
      struct {
        Float x;
        Float y;
      };
      char data[8];
    } the_pos = *(Pos*)&tanks_data[tank_data_index];
    const Vector2 tank_pos(the_pos.x, the_pos.y);
    tank_data_index += 4;

    const auto tank_rotation = *(Float*)&tanks_data[tank_data_index];
    tank_data_index += 2;

    const auto tank_state = *(uint16*)&tanks_data[tank_data_index];
    tank_data_index += 1;

    map_manager->SetTankState(user_id, tank_pos, tank_rotation, tank_state);
  }
  map_manager->EndSyncTankState();
}
void Xn::TankCraft::GameManagerComponent::SetUsersKillNumber(
    const uint& this_user_kill_number,
    const wchar* const& users_kill_number_data,
    const uint& users_kill_number_count) {
  uint users_kill_number_data_index = 0;

  for (uint users_index = 0; users_index < users_kill_number_count;
       ++users_index) {
    const auto user_id =
        *(uint*)&users_kill_number_data[users_kill_number_data_index];
    users_kill_number_data_index += 2;

    const auto users_kill_number =
        *(uint16*)&users_kill_number_data[users_kill_number_data_index];
    users_kill_number_data_index += 1;

    user_manager->GetUser(user_id)->kill_number = users_kill_number;
  }
}
void Xn::TankCraft::GameManagerComponent::SetEntitiesState(
    const wchar* const& entities_data, const uint& entity_count) {
  // TODO
}

void Xn::TankCraft::GameManagerComponent::AddUser(
    const uint user_id, const uint& user_name_length,
    const wchar* const& user_name, const Vector4& user_color,
    const uint& user_kill_number) {
  std::wstring name(user_name_length, '\0');
  memcpy(name.data(), user_name, size_t(2 * user_name_length));

  user_manager->AddUser(user_id, name, user_color, user_kill_number);
}
void Xn::TankCraft::GameManagerComponent::SetBulletState(const uint& id,
                                                         const Vector2& pos) {
  // TODO
}
void Xn::TankCraft::GameManagerComponent::SetTankBomb(const uint& user_id,
                                                      const Vector2& pos) {
  // TODO
}
void Xn::TankCraft::GameManagerComponent::SetLaser(
    const wchar* const& laser_data, const uint& point_count) {
  // TODO
}
