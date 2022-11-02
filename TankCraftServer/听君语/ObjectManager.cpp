#include "ObjectManager.h"
//*********************************************************
//
// ”…°∏¬Ì’—°π±‡–¥
//
//*********************************************************

#include "Component.h"
#include "ObjectManager-XnObject.h"

using namespace Xn;

ObjectManager::ObjectManager() : next_id_(1) {
  root_object_ = std::make_unique<XnObject>(0, L"no name", Vector2::ZERO,
                                            Vector2::ZERO, 0.0f, nullptr);
}

void ObjectManager::OnStart() { root_object_->OnStart(); }
void ObjectManager::OnUpdate() { root_object_->OnUpdate(); }
void ObjectManager::OnDestory() { root_object_->OnDestory(); }
XnObject* ObjectManager::GetRootObject() { return root_object_.get(); }

XnObject* ObjectManager::CreateXnObject() {
  return real_CreateXnObject(Vector2::ZERO, Vector2::ZERO, 0.0f,
                             root_object_.get());
}
XnObject* ObjectManager::CreateXnObject(const Vector2& position) {
  return real_CreateXnObject(position, Vector2::ZERO, 0.0f, root_object_.get());
}
XnObject* ObjectManager::CreateXnObject(const Vector2& position,
                                        XnObject* const& parent) {
  return real_CreateXnObject(position, Vector2::ZERO, 0.0f, parent);
}
XnObject* Xn::ObjectManager::CreateXnObject(const Vector2& position,
                                            const Vector2& scale,
                                            XnObject* const& parent) {
  return real_CreateXnObject(position, scale, 0.0f, parent);
}
XnObject* Xn::ObjectManager::CreateXnObject(const Vector2& position,
                                            const float& rotation,
                                            XnObject* const& parent) {
  return real_CreateXnObject(position, Vector2::ZERO, rotation, parent);
}
XnObject* Xn::ObjectManager::CreateXnObject(const Vector2& position,
                                            const Vector2& scale,
                                            const float& rotation,
                                            XnObject* const& parent) {
  return real_CreateXnObject(position, scale, rotation, parent);
}

inline XnObject* Xn::ObjectManager::real_CreateXnObject(
    const Vector2& position, const Vector2& scale, const float& rotation,
    XnObject* const& parent) {
  auto object = std::make_unique<XnObject>(next_id_, L"no name", position,
                                           scale, rotation, parent);
  ++next_id_;
  XnObject* xn_object = parent->AddChild(std::move(object));
  xn_object->OnStart();
  return xn_object;
}
