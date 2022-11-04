#pragma once

#include "Math.h"
#include "typedef.h"

namespace Xn {

namespace TankCraft {

struct UserData {
  UserData(const Int& user_id, const std::wstring& user_name,
           const Vector4& color, const Int& kill_number)
      : user_id(user_id),
        user_name(user_name),
        color(color),
        kill_number(kill_number) {}

  Int user_id;
  std::wstring user_name;
  Vector4 color;
  Int kill_number;
};

}  // namespace TankCraft

}  // namespace Xn
