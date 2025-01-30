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

#include "s2h/Engine/ecs.h"
#include "s2h/Engine/entity.h"

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

  window.Create(windowDesc, &eventBus);

  s2h::RendererBuilder builder{};
  builder.Set(s2h::RendererKind::SoftwareRasterizer);

  s2h::Application app{window.GetSurface(), builder.Build()};
  app.Initialize();

  eventBus.Subscribe(std::make_unique<nw::EventHandler<nw::WindowResizeEvent>>(
    [&window, &app](nw::WindowResizeEvent& e) {
      nw::Surface surface = window.CreateSurface(e.GetSize());
      app.OnWindowSizeChanged(surface);
      return true;
    }));

  // s2h::ECS ecs;
  // s2h::Entity e = ecs.Entity();
  // ecs.AddComponent(e, s2h::ecs::ComponentId{1});
  // ecs.AddComponent(e, s2h::ecs::ComponentId{2});
  // ecs.AddComponent(e, s2h::ecs::ComponentId{3});

  s2h::Timer timer;

  while (window.Update())
  {
    eventBus.Dispatch();
    timer.Tick(
      &s2h::Application::FixedUpdateLoop, &s2h::Application::UpdateLoop, &app);
    window.Present();
  }

  return 0;
}