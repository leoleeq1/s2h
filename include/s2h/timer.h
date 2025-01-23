#ifndef S2H_TIMER_H_
#define S2H_TIMER_H_

#include "s2h/Math/math.h"

#include <cstdint>
#include <exception>
#include <numeric>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace s2h
{
class Timer
{
 public:
  Timer()
    : elapsedTicks_(std::numeric_limits<uint64_t>::max()),
      totalTicks_(0),
      leftOverTicks_(0),
      frameCount_(0)
  {
    if (!QueryPerformanceFrequency(&frequency_))
    {
      throw std::exception();
    }

    if (!QueryPerformanceCounter(&lastTime_))
    {
      throw std::exception();
    }

    ticksPerSecond_ = static_cast<uint64_t>(frequency_.QuadPart);
    ticksPerMilliSecond_ = ticksPerSecond_ / 1000ULL;

    // 0.1s
    maxDelta_ = ticksPerSecond_ / 10ULL;

    // 0.02s
    fixedStepTicks_ = ticksPerMilliSecond_ * 20ULL;
  }

  [[nodiscard]] uint64_t GetElapsedTicks() const noexcept
  {
    return elapsedTicks_;
  }

  [[nodiscard]] double GetElapsedSeconds() const noexcept
  {
    return TicksToSeconds(elapsedTicks_);
  }

  [[nodiscard]] double GetElapsedMilliSeconds() const noexcept
  {
    return TicksToMilliSeconds(elapsedTicks_);
  }

  [[nodiscard]] uint64_t GetTotalTicks() const noexcept { return totalTicks_; }
  [[nodiscard]] double GetTotalSeconds() const noexcept
  {
    return TicksToSeconds(totalTicks_);
  }

  [[nodiscard]] uint32_t GetFrameCount() const noexcept { return frameCount_; }
  [[nodiscard]] uint32_t GetFPS() const noexcept
  {
    return static_cast<uint32_t>(ticksPerSecond_ / elapsedTicks_);
  }

  [[nodiscard]] double TicksToMilliSeconds(uint64_t ticks) const noexcept
  {
    return static_cast<double>(ticks) / ticksPerMilliSecond_;
  }

  [[nodiscard]] double TicksToSeconds(uint64_t ticks) const noexcept
  {
    return static_cast<double>(ticks) / ticksPerSecond_;
  }

  [[nodiscard]] uint64_t MilliSecondsToTicks(double seconds) const noexcept
  {
    return static_cast<uint64_t>(seconds * ticksPerMilliSecond_);
  }

  [[nodiscard]] uint64_t SecondsToTicks(double seconds) const noexcept
  {
    return static_cast<uint64_t>(seconds * ticksPerSecond_);
  }

  void ResetElapsedTime()
  {
    if (!QueryPerformanceCounter(&lastTime_))
    {
      throw std::exception();
    }

    leftOverTicks_ = 0;
  }

  template<typename TFixedUpdate, typename TUpdate>
    requires std::is_invocable_v<TFixedUpdate, float>
             && std::is_invocable_v<TUpdate, float>
  void Tick(const TFixedUpdate& fixedUpdate, const TUpdate& update)
  {
    LARGE_INTEGER currentTime;

    if (!QueryPerformanceCounter(&currentTime))
    {
      throw std::exception();
    }

    elapsedTicks_ =
      min(static_cast<uint64_t>(currentTime.QuadPart - lastTime_.QuadPart),
        maxDelta_);

    lastTime_ = currentTime;
    leftOverTicks_ += elapsedTicks_;
    totalTicks_ += elapsedTicks_;
    ++frameCount_;

    while (leftOverTicks_ >= fixedStepTicks_)
    {
      leftOverTicks_ -= fixedStepTicks_;
      fixedUpdate(static_cast<float>(TicksToSeconds(fixedStepTicks_)));
    }

    update(static_cast<float>(GetElapsedSeconds()));
  }

 private:
  LARGE_INTEGER frequency_;
  LARGE_INTEGER lastTime_;

  uint64_t ticksPerSecond_;
  uint64_t ticksPerMilliSecond_;
  uint64_t maxDelta_;
  uint64_t fixedStepTicks_;
  uint64_t elapsedTicks_;
  uint64_t totalTicks_;
  uint64_t leftOverTicks_;

  uint32_t frameCount_;
};
} // namespace s2h
#endif // S2H_TIMER_H_