#ifndef S2H_ENGINE_CAMERA_H_
#define S2H_ENGINE_CAMERA_H_

#include "s2h/Engine/Component/component.h"
#include "s2h/Math/matrix.h"
#include "s2h/Renderer/misc.h"

#include <cstdint>
#include <memory>

namespace s2h
{
class Texture;
class Camera : public Component
{
 public:
  bool FrustumCulling() const noexcept;
  mat4 GetViewMatrix() const noexcept;
  mat4 GetProjectionMatrix(const Texture& rt) const noexcept;
  void Render();

  s2h::RenderTarget GetRenderTarget() const noexcept;

 private:
  s2h::RenderTarget renderTarget_;
  float far_ = 1000.0f;
  float near_ = 1.0f;
  float fov_ = 60.0f;
};
} // namespace s2h

#endif // S2H_CAMERA_H_