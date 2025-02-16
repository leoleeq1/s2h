#include "application.h"

#include "nw/Event/ApplicationEvent.h"
#include "nw/Event/event.h"
#include "nw/Event/event_bus.h"
#include "nw/color.h"
#include "nw/window.h"
#include "nw/window_desc.h"
#include "s2h/Renderer/renderer.h"
#include "s2h/Renderer/renderer_builder.h"
#include "s2h/Renderer/software_rasterizer.h"
#include "s2h/Renderer/vertex.h"
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

  nw::WindowDesc windowDesc{
    .size = {800, 600},
    .mode = nw::WindowMode::Windowed,
  };

  nw::Surface surface{windowDesc.size};
  window.Create(windowDesc, &eventBus, &surface);

  s2h::RendererBuilder builder{};
  builder.Set(s2h::RendererKind::SoftwareRasterizer);

  s2h::Application app{&surface, builder.Build()};
  app.Initialize();

  eventBus.Subscribe(std::make_unique<nw::EventHandler<nw::WindowResizeEvent>>(
    [&app, &surface](nw::WindowResizeEvent& e) {
      surface.Resize(e.GetSize());
      app.OnWindowSizeChanged();
      return true;
    }));

  s2h::Timer timer;

  while (window.Update())
  {
    eventBus.Dispatch();
    timer.Tick(
      &s2h::Application::FixedUpdateLoop, &s2h::Application::UpdateLoop, &app);
    window.Present();
    std::println("{}ms", timer.GetElapsedMilliSeconds());
  }

  return 0;
}