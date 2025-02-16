#ifndef S2H_APPLICATION_H_
#define S2H_APPLICATION_H_

#include "nw/surface.h"
#include "s2h/Engine/OOP/scene.h"
#include "s2h/Engine/resource_manager.h"

#include <memory>

namespace s2h
{
class RendererBase;
class Application
{
 public:
  Application(
    nw::Surface *surface, std::unique_ptr<s2h::RendererBase> renderer);
  ~Application() = default;
  Application(const Application&) = delete;
  Application& operator=(const Application&) = delete;
  Application(Application&&) = delete;
  Application& operator=(Application&&) = delete;

  void OnWindowSizeChanged();

  void Initialize();

  static void UpdateLoop(float dt, Application *app);
  static void FixedUpdateLoop(float dt, Application *app);

 private:
  void FixedUpdate(float dt);
  void Update(float dt);
  void LateUpdate(float dt);
  void Render();

  s2h::ResourceManager resourceManager_;
  s2h::Scene scene_;
  nw::Surface *surface_;
  std::unique_ptr<s2h::RendererBase> renderer_;
};
} // namespace s2h

#endif // S2H_APPLICATION_H_