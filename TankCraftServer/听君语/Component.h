#pragma once
//*********************************************************
//
// 由「马昭」编写
//
//*********************************************************

#include <string>

namespace Xn {

class XnObject;

class Component {
  friend class XnObject;

 public:
  // 组件类型，其实就是字符串
  typedef std::wstring ComponentType;

 protected:
  Component(ComponentType component_type) : component_type_(component_type) {}

 public:
  // 取得所挂载的目标对象
  XnObject* const& GetXnObject() { return object_; }
  // 取得组件类型
  ComponentType GetComponentType() { return component_type_; }

 public:
  // 重载此函数而不是使用构造函数初始化非全局内容
  // 组件可能不被释放，并通过调用OnDestory()OnStart()来达到重复使用的效果
  virtual void OnStart() {}
  // 重载此函数而不是使用析构函数初始化非全局内容
  // 组件可能不被释放，并通过调用OnDestory()OnStart()来达到重复使用的效果
  virtual void OnDestory() {}
  // 将在每帧被调用
  virtual void OnUpdate() {}

 private:
  XnObject* object_ = nullptr;
  const ComponentType component_type_;
};

}  // namespace Xn
