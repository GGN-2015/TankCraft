#include "TankCraft_UserManager_Component.h"

using namespace Xn;
using namespace TankCraft;

void Xn::TankCraft::UserManagerComponent::OnStart() {}
void Xn::TankCraft::UserManagerComponent::OnDestory() { RemoveAllUser(); }

UserData* const& Xn::TankCraft::UserManagerComponent::GetUser(
    const Int& user_id) {
  auto user = users_.find(user_id);
  if (user == users_.end()) return nullptr;
  return user->second.get();
}
UserData* const& Xn::TankCraft::UserManagerComponent::AddUser(
    Int user_id, std::wstring user_name, Vector4 color, Int kill_number) {
  auto user_data =
      std::make_unique<UserData>(user_id, user_name, color, kill_number);
  auto ret = user_data.get();
  users_.insert({user_id, std::move(user_data)});
  return ret;
}
void Xn::TankCraft::UserManagerComponent::RemoveUser(const Int& user_id) {
  users_.erase(user_id);
}
void Xn::TankCraft::UserManagerComponent::RemoveAllUser() { users_.clear(); }
