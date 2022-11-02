#include "XnTime.h"
//*********************************************************
//
// ”…°∏¬Ì’—°π±‡–¥
//
//*********************************************************

using namespace std::chrono;
using namespace Xn;

Time::Timer::Timer() { last_ = steady_clock::now(); }

float Time::Timer::Mark() {
  const auto old = last_;
  last_ = steady_clock::now();
  const duration<float> frame_time = last_ - old;
  return frame_time.count();
}

float Time::Timer::Peek() const {
  return duration<float>(steady_clock::now() - last_).count();
}

Time::Time() : delta_time_(0.1f), time_from_start_(0) {}

float Time::GetTimeFromStart() noexcept { return time_from_start_; }

float Time::GetDeltaTime() noexcept { return delta_time_; }

void Time::NextFrame() {
  delta_time_ = timer_.Mark();
  time_from_start_ += delta_time_;
}
