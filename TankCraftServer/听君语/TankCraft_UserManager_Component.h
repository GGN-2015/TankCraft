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

 public:
  UserData* const GetUser(const Int& user_id);

 private:
  UserData* const AddUser(const Int& user_id, const std::wstring& user_name,
                          const Vector4& color, const Int& kill_number);
  void RemoveUser(const Int& user_id);
  void RemoveAllUser();

 private:
  std::map<Int, std::unique_ptr<UserData>> users_;
};

}  // namespace TankCraft

}  // namespace Xn
