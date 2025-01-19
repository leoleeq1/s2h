#include "nw/Event/ApplicationEvent.h"
#include "nw/Event/event.h"
#include "nw/Event/event_bus.h"
#include "nw/window.h"
#include "s2h/vector.h"
#include <array>
#include <memory>
#include <print>

int main()
{
  nw::Window nw;
  nw::EventBus eventBus{};
  // nw.Create({.mode = nw::WindowMode::Windowed}, &eventBus);

  // while (nw.Update())
  //{
  //   eventBus.Dispatch();
  // }

  std::array<float, 4> arr{0.0f, 1.0f, 2.f, 3.f};
  v2f v3{4.f, 5.f};
  v2f v4{-1.f, -3.f};
  v4f v5{1.f, 2.f, 3.f, 4.f};
  vec<float, 4> v2{arr};
  vec<float, 4> v{7.f, 8.f, 9.f};

  for (auto& value : (3.5f * v3).v)
  {
    std::print("{} ", value);
  }
  std::println();
  for (auto& value : (-v3).v)
  {
    std::print("{} ", value);
  }
  std::println();
  for (auto& value : (v5 * 1.23f).v)
  {
    std::print("{} ", value);
  }
  std::println();

  for (auto& value : v2.v)
  {
    std::print("{} ", value);
  }
  std::println();

  for (auto& value : v.v)
  {
    std::print("{} ", value);
  }
  std::println();
  return 0;
}