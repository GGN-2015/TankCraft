#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

namespace Xn {

#define CAN_NOT_COPY_THIS_CLASS(ClassName) \
  ClassName(const ClassName &) = delete;   \
  ClassName &operator=(const ClassName &) = delete

#define SINGLETON_HELPER(ClassName)   \
  friend class Singleton<ClassName>;  \
                                      \
 public:                              \
  CAN_NOT_COPY_THIS_CLASS(ClassName); \
                                      \
 private:                             \
  ClassName() = default

// ����ģ����
// ʹ��������
//   class T :public Singleton<T> {
//     SINGLETON_HELPER(T);
//   }
template <typename T>
class Singleton {
 public:
  static T &Get() {
    static T instance;
    return instance;
  }

  virtual ~Singleton() = default;
  CAN_NOT_COPY_THIS_CLASS(Singleton);

 protected:
  Singleton() noexcept = default;
};

}  // namespace Xn
