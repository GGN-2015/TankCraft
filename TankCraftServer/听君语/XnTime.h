#pragma once
//*********************************************************
//
// �ɡ����ѡ���д
//
//*********************************************************

#include <chrono>

namespace Xn {

class ������;

class Time {
  friend ������;

 public:
  class Timer {
   public:
    Timer();
    float Mark();
    float Peek() const;

   private:
    std::chrono::steady_clock::time_point last_;
  };

 public:
  Time();

  float GetDeltaTime() noexcept;
  float GetTimeFromStart() noexcept;

 private:
  void NextFrame();

 public:
  Timer timer_;

 private:
  float delta_time_;
  float time_from_start_;
};

}  // namespace Xn
