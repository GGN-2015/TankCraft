#include "ObjectManager-XnObject.h"
//*********************************************************
//
// ÓÉ¡¸ÂíÕÑ¡¹±àÐ´
//
//*********************************************************

#include "Component.h"
#include "RenderComponent.h"

using namespace Xn;

XnObject::XnObject(const uint &id, const std::wstring &name, const Vector2 &pos,
                   const Vector2 &scale, const float &rotation,
                   XnObject *const &parent)
    : id_(id),
      name_(name),
      pos_(pos),
      scale_(scale),
      rotation_(rotation),
      parent_(parent),
      is_active_(true) {}

void XnObject::OnStart() {}
void XnObject::OnUpdate() {
  for (auto &item : components_) item.second->OnUpdate();
  for (auto &item : children_)
    if (item.second->is_active_) item.second->OnUpdate();
}
void XnObject::OnDestory() {
  for (auto &item : children_)
    if (item.second->is_active_) item.second->OnDestory();
  for (auto &item : components_) item.second->OnDestory();
}

void Xn::XnObject::OnRender() {
  if (render_component_.get())
    render_component_.get()->StartRender(pos_, scale_, rotation_);
  for (auto &item : children_)
    if (item.second->is_active_) item.second->OnRender();
  if (render_component_.get()) render_component_.get()->OverRender();
}

const uint XnObject::GetId() { return id_; }
const std::wstring XnObject::GetName() { return name_; }

Vector2 XnObject::GetPos() { return pos_; }
Vector2 Xn::XnObject::GetScale() { return scale_; }
float Xn::XnObject::GetRotation() { return rotation_; }

Component *XnObject::GetComponent(
    const Component::ComponentType &component_type) {
  auto iter = components_.find(component_type);
  if (iter == components_.end())
    return nullptr;
  else
    return iter->second.get();
}
Component *XnObject::AddComponent(std::unique_ptr<Component> component) {
  component->object_ = this;

  auto type = component->GetComponentType();
  auto the_component = component.get();
  components_.insert({type, std::move(component)});

  the_component->OnStart();

  return the_component;
}
void XnObject::RemoveComponet(const Component::ComponentType &component_type) {
  auto iter = components_.find(component_type);
  if (iter == components_.end()) return;
  iter->second->OnDestory();

  components_.erase(iter);
}

RenderComponent *Xn::XnObject::SetRenderComponent(
    std::unique_ptr<class RenderComponent> render_component) {
  render_component->object_ = this;
  render_component_ = std::move(render_component);
  render_component_->OnStart();
  return render_component_.get();
}
void Xn::XnObject::RemoveRenderComponet() {
  if (render_component_.get()) render_component_->OnDestory();
  render_component_ = nullptr;
}

XnObject *XnObject::AddChild(std::unique_ptr<XnObject> child) {
  XnObject *xn_object = child.get();
  const uint id = child->GetId();
  return children_.insert({id, std::move(child)}).first->second.get();
}
void Xn::XnObject::RemoveChild(XnObject *&child) {
  RemoveChild(child->GetId());
  child = nullptr;
}
void Xn::XnObject::RemoveChild(const uint &child_id) {
  children_.erase(child_id);
}