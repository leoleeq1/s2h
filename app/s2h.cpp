#include "nw/Event/event.h"
#include "nw/Event/event_bus.h"
#include "nw/color.h"
#include "nw/window.h"
#include "nw/window_desc.h"
#include "s2h/timer.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <print>

int main()
{
  nw::Window window;
  nw::EventBus eventBus{};
  s2h::Timer timer;

  nw::WindowDesc windowDesc{
    .size = {800, 600},
    .mode = nw::WindowMode::Windowed,
  };

  window.Create(windowDesc, &eventBus);

  int i = 0;
  while (window.Update())
  {
    eventBus.Dispatch();
    timer.Tick([](float dt) { std::println("{}s FixedUpdate Called!", dt); },
      [](float dt) { std::println("{}s Update Called!", dt); });
    nw::Surface surface = window.GetSurface();
    std::fill_n(
      surface.pixels, surface.Length(), nw::Color(255, 0, (i++) % 256, 0));
    window.Present();
    std::println("fps: {}, dt: {}ms, tt: {}s", timer.GetFPS(),
      timer.GetElapsedMilliSeconds(), timer.GetTotalSeconds());
  }

  return 0;
}