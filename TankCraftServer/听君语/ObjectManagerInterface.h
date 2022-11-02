#pragma once
//*********************************************************
//
// ”…°∏¬Ì’—°π±‡–¥
//
//*********************************************************

#include "Math.h"
#include "typedef.h"

#undef GetObject

namespace Xn {

class XnObject;

class ObjectManagerInterface {
 public:
  virtual void OnStart() = 0;
  virtual void OnUpdate() = 0;
  virtual void OnDestory() = 0;

  virtual XnObject* GetRootObject() = 0;
  virtual XnObject* CreateXnObject() = 0;
  virtual XnObject* CreateXnObject(const Vector2& position) = 0;
  virtual XnObject* CreateXnObject(const Vector2& position,
                                   XnObject* const& parent) = 0;
  virtual XnObject* CreateXnObject(const Vector2& position,
                                   const Vector2& scale,
                                   XnObject* const& parent) = 0;
  virtual XnObject* CreateXnObject(const Vector2& position,
                                   const float& rotation,
                                   XnObject* const& parent) = 0;
  virtual XnObject* CreateXnObject(const Vector2& position,
                                   const Vector2& scale, const float& rotation,
                                   XnObject* const& parent) = 0;
};

}  // namespace Xn
