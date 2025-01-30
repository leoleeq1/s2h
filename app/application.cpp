#include "application.h"
#include "nw/color.h"
#include "nw/surface.h"
#include "s2h/Engine/Component/camera.h"
#include "s2h/Engine/Component/mesh_renderer.h"
#include "s2h/Engine/Component/transform.h"
#include "s2h/Engine/gameobject.h"
#include "s2h/Engine/resource_manager.h"
#include "s2h/Engine/scene.h"
#include "s2h/Renderer/buffer.h"
#include "s2h/Renderer/renderer.h"
#include "s2h/Resource/mesh.h"
#include "s2h/Resource/resource.h"
#include "s2h/Resource/texture.h"

#include <array>
#include <print>

namespace s2h
{
Application::Application(
  nw::Surface surface, std::unique_ptr<s2h::RendererBase> renderer)
  : surface_{surface},
    renderer_{std::move(renderer)}
{
  OnWindowSizeChanged(surface);
}

void Application::OnWindowSizeChanged(const nw::Surface& surface)
{
  std::array descriptors = {
    TextureDesc{.format = TextureFormat::A8R8G8B8,
                .width = surface.width,
                .height = surface.height},
  };

  renderer_->Initialize(descriptors);
  surface_ = surface;
}

void Application::Initialize()
{
  s2h::Mesh *cubeMesh =
    resourceManager_.Add<s2h::Mesh>(std::make_unique<s2h::Mesh>(
      std::array{
        s2h::Vertex(s2h::v3f{-0.5f, -0.5f, -0.5f}),
        s2h::Vertex(s2h::v3f{-0.5f, -0.5f, +0.5f}),
        s2h::Vertex(s2h::v3f{+0.5f, -0.5f, +0.5f}),
        s2h::Vertex(s2h::v3f{+0.5f, -0.5f, -0.5f}),
        s2h::Vertex(s2h::v3f{-0.5f, +0.5f, -0.5f}),
        s2h::Vertex(s2h::v3f{-0.5f, +0.5f, +0.5f}),
        s2h::Vertex(s2h::v3f{+0.5f, +0.5f, +0.5f}),
        s2h::Vertex(s2h::v3f{+0.5f, +0.5f, -0.5f}),
      },
      std::array{0, 1, 2, 0, 2, 3, 0, 4, 7, 0, 7, 3, 1, 5, 4, 1, 4, 0, 2, 6, 5,
        2, 5, 1, 3, 7, 6, 3, 6, 2, 4, 5, 6, 4, 6, 7}));

  s2h::GameObject *cube = scene_.CreateGameObject("cube");
  s2h::MeshRenderer *mr = cube->AddComponent<s2h::MeshRenderer>();
  cube->GetTransform().Scale = s2h::v3f::One() * 10;
  mr->SetMesh(cubeMesh);

  s2h::GameObject *camGo = scene_.CreateGameObject("cam");
  s2h::Camera *cam = camGo->AddComponent<s2h::Camera>();
  cam->GetGameObject().GetTransform().Position = s2h::v3f(0.0f, 0.0f, -50.0f);
  s2h::v3f scale = camGo->GetTransform().Scale;
}

void Application::FixedUpdateLoop(float dt, Application *app)
{
  app->FixedUpdate(dt);
}

void Application::UpdateLoop(float dt, Application *app)
{
  app->Update(dt);
  app->LateUpdate(dt);
  app->Render();
}

void Application::FixedUpdate(float dt) {}

void Application::Update(float dt)
{
  s2h::GameObject *cube = scene_.FindGameObjectWithName("cube");
  s2h::GameObject *camGo = scene_.FindGameObjectWithName("cam");

  //   cube->GetTransform().Position += s2h::v3f::Basis(0) * dt;
  cube->GetTransform().LocalRotation += s2h::v3f::Basis(1) * 180 * dt;
  camGo->GetTransform().LocalRotation += s2h::v3f(0, dt, 0.0f);
}

void Application::LateUpdate(float dt) {}

void Application::Render()
{
  std::span<uint8_t> clearColor(
    reinterpret_cast<uint8_t *>(&nw::Color::white), sizeof(nw::Color));

  auto cameras = scene_.FindComponents<s2h::Camera>();
  auto meshRenderers = scene_.FindComponents<s2h::MeshRenderer>();
  s2h::ConstantBuffer cb;

  for (auto& camera : cameras)
  {
    s2h::RenderTarget target = camera->GetRenderTarget();
    renderer_->Clear(target, clearColor);

    cb.SetViewMatrix(camera->GetViewMatrix());
    cb.SetProjectionMatrix(camera->GetProjectionMatrix(
      renderer_->GetRenderTexture(camera->GetRenderTarget())));

    for (auto& mr : meshRenderers)
    {
      auto mesh = mr->GetMesh();
      cb.SetModelMatrix(mr->GetGameObject().GetTransform().GetModelMatrix());
      renderer_->DrawIndexed(
        camera->GetRenderTarget(), cb, mesh->GetVB(), mesh->GetIB());
    }
  }

  renderer_->Submit(
    s2h::RenderTarget{0}, reinterpret_cast<uint8_t *>(surface_.pixels));
}
} // namespace s2h
