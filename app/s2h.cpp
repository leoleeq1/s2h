#include "nw/Event/event.h"
#include "nw/Event/event_bus.h"
#include "nw/color.h"
#include "nw/window.h"
#include "nw/window_desc.h"
#include "s2h/Renderer/renderer.h"
#include "s2h/Renderer/renderer_builder.h"
#include "s2h/Renderer/software_rasterizer.h"
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
  s2h::RendererBuilder builder{};

  nw::WindowDesc windowDesc{
    .size = {800, 600},
    .mode = nw::WindowMode::Windowed,
  };

  window.Create(windowDesc, &eventBus);
  std::unique_ptr<s2h::IRenderer> renderer =
    builder.Set(s2h::RendererKind::SoftwareRasterizer)
      .Set(window.GetSurface())
      .Build();

  while (window.Update())
  {
    eventBus.Dispatch();
    timer.Tick([](float dt) {}, [](float dt) {});
    renderer->Clear(nw::Color::white);
    renderer->Draw();
    window.Present();
  }

  return 0;
}