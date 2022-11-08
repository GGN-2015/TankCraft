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

class Text_RenderComponent;

namespace TankCraft {

class MapManagerComponent;
class UserManagerComponent;
class NetManager_Component;
class NetMessageBaseDataBuffer;

// 以x（Vector2::X）为前方向
class GameManagerComponent : public Component {
 public:
  enum class GameState {
    NoConnect,
    WaitForConnect,
    Connected,
    WaitForLogin,
    Gaming,
  };

 public:
  GameManagerComponent() : Component(L"TanksFight_GameManagerComponent") {}

  virtual void OnStart() override;
  virtual void OnUpdate() override;
  virtual void OnDestory() override;

 public:
  void ConnectToServer(const std::wstring ipV4, const std::wstring port);
  void Login(const std::wstring user_name);

 private:
  void NetMessageDeal(NetMessageBaseDataBuffer *const &server_to_client);
  void InternalCommunicationMessage(const uint &code);
  void SetMap(const wchar *const &map_data, const uint &x_side_length,
              const uint &y_side_length);
  void AddUsers(const wchar *const &users_data, const uint &user_count);
  void SetTanksState(const wchar *const &tanks_data, const uint &tank_count);
  void SetUsersKillNumber(const uint &this_user_kill_number,
                          const wchar *const &users_kill_number_data,
                          const uint &users_kill_number_count);
  void SetEntitiesState(const wchar *const &entities_data,
                        const uint &entity_count);
  void DealLoginMessage(const uint &login_error_code, const uint &user_id);

  void TrySendKeyMessage();

 private:
  void AddUser(const uint user_id, const uint &user_name_length,
               const wchar *const &user_name, const Vector4 &user_color,
               const uint &user_kill_number);
  void SetBulletState(const uint &id, const Vector2 &pos);
  void SetTankBomb(const uint &user_id, const Vector2 &pos);
  void SetLaser(const wchar *const &laser_data, const uint &point_count);

 private:
  GameState game_state_ = GameState::NoConnect;

  Text_RenderComponent *error_message_text = nullptr;

  MapManagerComponent *map_manager_ = nullptr;
  UserManagerComponent *user_manager_ = nullptr;
  NetManager_Component *net_manager_ = nullptr;
};

}  // namespace TankCraft

}  // namespace Xn
