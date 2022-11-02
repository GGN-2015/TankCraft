#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include <string>

namespace Xn {

class XnObject;

class Component {
  friend class XnObject;

 public:
  // ������ͣ���ʵ�����ַ���
  typedef std::wstring ComponentType;

 protected:
  Component(ComponentType component_type) : component_type_(component_type) {}

 public:
  // ȡ�������ص�Ŀ�����
  XnObject* const& GetXnObject() { return object_; }
  // ȡ���������
  ComponentType GetComponentType() { return component_type_; }

 public:
  // ���ش˺���������ʹ�ù��캯����ʼ����ȫ������
  // ������ܲ����ͷţ���ͨ������OnDestory()OnStart()���ﵽ�ظ�ʹ�õ�Ч��
  virtual void OnStart() {}
  // ���ش˺���������ʹ������������ʼ����ȫ������
  // ������ܲ����ͷţ���ͨ������OnDestory()OnStart()���ﵽ�ظ�ʹ�õ�Ч��
  virtual void OnDestory() {}
  // ����ÿ֡������
  virtual void OnUpdate() {}

 private:
  XnObject* object_ = nullptr;
  const ComponentType component_type_;
};

}  // namespace Xn
