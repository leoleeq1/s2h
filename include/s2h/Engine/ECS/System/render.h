#ifndef S2H_ENGINE_ECS_SYSTEM_RENDER_H_
#define S2H_ENGINE_ECS_SYSTEM_RENDER_H_

#include "s2h/Engine/ECS/Component/camera.h"
#include "s2h/Engine/ECS/Component/transform.h"
#include "s2h/Engine/ECS/System/system.h"
#include "s2h/Engine/ECS/System/transform.h"
#include "s2h/Engine/ECS/ecs.h"
#include "s2h/Math/matrix.h"
#include "s2h/Math/vector.h"
#include "s2h/Renderer/renderer.h"
#include "s2h/Resource/texture.h"

#include <any>
#include <array>
#include <print>

namespace s2h::ecs::RenderSystem
{
mat4 GetViewMatrix(s2h::v3f position, s2h::v3f rotation) noexcept
{
  mat4 t = mat4{
    v4f::Basis(0),
    v4f::Basis(1),
    v4f::Basis(2),
    v4f{-position, 1.0f},
  };
  mat4 r =
    s2h::Transpose(s2h::ecs::TransformSystem::GetRotationMatrix(rotation));
  return t * r;
}

mat4 GetProjectionMatrix(
  const s2h::ecs::CameraComponent& camera, const s2h::Texture& rt) noexcept
{
  float rAspect = rt.GetHeight() / static_cast<float>(rt.GetWidth());
  float d = 1.0f / s2h::tan((s2h::numbers::fDeg2Rad * camera.fov) * 0.5f);
  float n = camera.near;
  float f = camera.far;
  return mat4{
    v4f{d * rAspect, 0.0f, 0.0f,                   0.0f },
    v4f{0.0f,        d,    0.0f,                   0.0f },
    v4f{0.0f,        0.0f, -(n + f) / (f - n),     -1.0f},
    v4f{0.0f,        0.0f, -(2 * n * f) / (f - n), 0.0f },
  };
}
} // namespace s2h::ecs::RenderSystem

#endif // S2H_ENGINE_ECS_SYSTEM_RENDER_H_