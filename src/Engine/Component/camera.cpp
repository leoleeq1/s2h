#include "s2h/Engine/Component/camera.h"
#include "s2h/Engine/Component/transform.h"
#include "s2h/Engine/gameobject.h"
#include "s2h/Resource/texture.h"

#include <cstdint>

namespace s2h
{
bool Camera::FrustumCulling() const noexcept
{
  return true;
}

mat4 Camera::GetViewMatrix() const noexcept
{
  return gameObject_->GetTransform().GetViewMatrix();
}

mat4 Camera::GetProjectionMatrix(const Texture& rt) const noexcept
{
  // float aspect = static_cast<float>(rt.GetWidth()) / rt.GetHeight();
  // float tangent = s2h::tan(s2h::numbers::fDeg2Rad * fov_ / 2);
  // float right = near_ * tangent; // half width of near plane
  // float top = right / aspect;

  // return mat4{
  //   v4f{near_ / right, 0.0f,        0.0f, 0.0f }, v4f{0.0f,          near_ /
  //   top, 0.0f,                                 0.0f }, v4f{0.0f, 0.0f,
  //   -(near_ + far_) / (far_ - near_),     -1.0f}, v4f{0.0f,          0.0f,
  //   -(2 * near_ * far_) / (far_ - near_), 0.0f },
  // };

  float rAspect = rt.GetHeight() / static_cast<float>(rt.GetWidth());
  float d = 1.0f / s2h::tan((s2h::numbers::fDeg2Rad * fov_) * 0.5f);
  return mat4{
    v4f{d * rAspect, 0.0f, 0.0f,                                 0.0f },
    v4f{0.0f,        d,    0.0f,                                 0.0f },
    v4f{0.0f,        0.0f, -(near_ + far_) / (far_ - near_),     -1.0f},
    v4f{0.0f,        0.0f, -(2 * near_ * far_) / (far_ - near_), 0.0f },
  };
}

s2h::RenderTarget Camera::GetRenderTarget() const noexcept
{
  return renderTarget_;
}
} // namespace s2h
