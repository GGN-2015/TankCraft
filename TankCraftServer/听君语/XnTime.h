#pragma once
//*********************************************************
//
// ÓÉ¡¸ÂíÕÑ¡¹±àĞ´
//
//*********************************************************

#include <chrono>

namespace Xn {

class Ìı¾ıÓï;

class Time {
  friend Ìı¾ıÓï;

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
