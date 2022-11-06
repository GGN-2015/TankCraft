#include "TankCraft_GameManager_Component.h"

#include "ObjectManager-XnObject.h"
#include "ObjectManager.h"
#include "TankCraft_LoginComponent.h"
#include "TankCraft_MapManager_Component.h"
#include "TankCraft_NetManager_Component.h"
#include "TankCraft_UserManager_Component.h"
#include "听君语.h"

void Xn::TankCraft::GameManagerComponent::OnStart() {
  user_manager = (UserManagerComponent*)GetXnObject()->AddComponent(
      std::make_unique<UserManagerComponent>());
  net_manager = (NetManager_Component*)GetXnObject()->AddComponent(
      std::make_unique<NetManager_Component>());

  map_manager = (MapManagerComponent*)听君语::Get()
                    .GetObjectManager()
                    ->CreateXnObject(Vector2::ZERO, GetXnObject())
                    ->AddComponent(std::make_unique<MapManagerComponent>());
  听君语::Get()
      .GetObjectManager()
      ->CreateXnObject(Vector2::ZERO, GetXnObject())
      ->AddComponent(std::make_unique<LoginComponent>(this));

  net_manager->ConnectToServer(49, 140, 58, 248, 10086);
}
void Xn::TankCraft::GameManagerComponent::OnUpdate() {
  if (const auto buffer = net_manager->TryGetServerToClientMessageBuffer()) {
    NetMessageDeal(net_manager->TryGetClientToServerMessageBuffer(), buffer);
  }
}
void Xn::TankCraft::GameManagerComponent::OnDestory() {
  GetXnObject()->RemoveAllChild();
}

void Xn::TankCraft::GameManagerComponent::NetMessageDeal(
    NetMessageBaseDataBuffer* const& client_to_server,
    NetMessageBaseDataBuffer* const& server_to_client) {
  for (auto message = server_to_client->Pop(); message.get();
       message = server_to_client->Pop()) {
    const wchar* data = message->data;
    uint index = 0;

    const uint msg_type = *(uint16*)&data[0];
    switch (msg_type) {
      case 65535: {
        const uint error_code = *(uint16*)&data[2];
        InternalCommunicationMessage(error_code);
      } break;

      case 0: {
        const uint return_x = *(uint16*)&data[2];
        // TODO Ping不管
      } break;

      case 1: {
        const uint login_error_code = *(uint16*)&data[2];
        const uint user_id = *(uint*)&data[3];
        // TODO
      } break;

      case 2: {
        const uint wh = *(uint16*)&data[2];
        const uint width = (wh >> 4) & 4;
        const uint height = wh & 4;
        SetMap(&data[3], width, height);
      } break;

      case 3: {
        const uint user_count = *(uint16*)&data[2];
        AddUsers(&data[3], user_count);
      } break;

      case 4: {
        const uint tank_count = *(uint16*)&data[2];
        SetTanksState(&data[3], tank_count);
      } break;

      case 5: {
        const uint this_user_kill_number = *(uint16*)&data[2];
        const uint user_kill_number_count = *(uint16*)&data[3];
        SetUsersKillNumber(this_user_kill_number, &data[4],
                           user_kill_number_count);
      } break;

      case 6: {
        // 按键反馈消息，不管
      } break;

      case 7: {
        const uint entitiy_count = *(uint16*)&data[2];
        SetEntitiesState(&data[3], entitiy_count);
      } break;

      default:
        break;
    }
  }
}

void Xn::TankCraft::GameManagerComponent::InternalCommunicationMessage(
    const uint& code) {
  OutputDebugString(std::to_wstring(code).data());
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

  const auto user_data =
      user_manager->AddUser(user_id, name, user_color, user_kill_number);

  map_manager->TryBindUser(user_data);
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
