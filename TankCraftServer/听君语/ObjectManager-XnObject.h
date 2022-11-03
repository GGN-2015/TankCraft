#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include <map>

#include "Component.h"
#include "ForDebug-XnException.h"
#include "Math.h"
#include "RenderComponent.h"
#include "typedef.h"

namespace Xn {

class XnObject {
  friend class ObjectManagerInterface;
  friend class ObjectManager;

 public:
  // *警告*仅应由ObjectManager调用
  XnObject(const uint& id, const std::wstring& name, const Vector2& pos,
           const Vector2& scale, const float& rotation,
           XnObject* const& parent);

  virtual void OnStart();
  virtual void OnUpdate();
  virtual void OnDestory();

  void OnRender();

  const uint GetId();
  const std::wstring GetName();
  Vector2 GetPos();
  Vector2 GetScale();
  float GetRotation();
  Component* GetComponent(const Component::ComponentType& component_type);

  Component* AddComponent(std::unique_ptr<Component> component);
  void RemoveComponet(const Component::ComponentType& component_type);
  RenderComponent* SetRenderComponent(
      std::unique_ptr<RenderComponent> render_component);
  void RemoveRenderComponet();

 private:
  XnObject* AddChild(std::unique_ptr<XnObject> child);
  void RemoveChild(XnObject*& child);
  void RemoveChild(const uint& child_id);

 public:
  Vector2 pos_;     // 物体的位置
  Vector2 scale_;   // 物体的缩放
  float rotation_;  // 物体的旋转

 private:
  const uint id_;
  std::wstring name_;

  bool is_active_;

  XnObject* const parent_;
  std::map<uint, std::unique_ptr<XnObject>> children_;

  std::map<Component::ComponentType, std::unique_ptr<Component>> components_;
  std::unique_ptr<RenderComponent> render_component_;
};

}  // namespace Xn
