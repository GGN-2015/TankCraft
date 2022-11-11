#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include <map>
#include <memory>

#include "Component.h"
#include "Math.h"
#include "TankCraft_UserData.h"
#include "typedef.h"

namespace Xn {

namespace TankCraft {

class GameManagerComponent;

// ��x��Vector2::X��Ϊǰ����
class UserManagerComponent : public Component {
  friend class GameManagerComponent;

 public:
  UserManagerComponent() : Component(L"TanksFight_UserManagerComponent") {}

  virtual void OnStart() override;
  virtual void OnDestory() override;

 private:
  UserData *const GetUser(const Int &user_id);
  UserData *const AddUser(Int user_id, std::wstring user_name, Vector4 color,
                           Int kill_number);
  void RemoveUser(const Int &user_id);
  void RemoveAllUser();

 private:
  std::map<Int, std::unique_ptr<UserData>> users_;
};

}  // namespace TankCraft

}  // namespace Xn
