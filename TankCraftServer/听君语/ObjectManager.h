#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include <memory>

#include "ObjectManagerInterface.h"
#include "SingletonBaseClass.h"
#include "typedef.h"

namespace Xn {

class ObjectManager : public ObjectManagerInterface {
 public:
  ObjectManager();

  // 通过 ObjectManagerInterface 继承
  virtual void OnStart() override;
  virtual void OnUpdate() override;
  virtual void OnDestory() override;

  virtual XnObject* GetRootObject() override;

  virtual XnObject* CreateXnObject() override;
  virtual XnObject* CreateXnObject(const Vector2& position) override;
  virtual XnObject* CreateXnObject(const Vector2& position,
                                   XnObject* const& parent) override;
  virtual XnObject* CreateXnObject(const Vector2& position,
                                   const Vector2& scale,
                                   XnObject* const& parent) override;
  virtual XnObject* CreateXnObject(const Vector2& position,
                                   const float& rotation,
                                   XnObject* const& parent) override;
  virtual XnObject* CreateXnObject(const Vector2& position,
                                   const Vector2& scale, const float& rotation,
                                   XnObject* const& parent) override;

 private:
  inline XnObject* real_CreateXnObject(const Vector2& position,
                                       const Vector2& scale,
                                       const float& rotation,
                                       XnObject* const& parent);

 private:
  uint next_id_;

  std::unique_ptr<XnObject> root_object_;

  CAN_NOT_COPY_THIS_CLASS(ObjectManager);
};

}  // namespace Xn
