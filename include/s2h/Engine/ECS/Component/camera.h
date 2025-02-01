#ifndef S2H_ENGINE_ECS_COMPONENT_CAMERA_H_
#define S2H_ENGINE_ECS_COMPONENT_CAMERA_H_

#include "s2h/Renderer/misc.h"

namespace s2h::ecs
{
struct CameraComponent
{
  s2h::RenderTarget renderTarget;
  float far = 1000.0f;
  float near = 1.0f;
  float fov = 60.0f;
};
} // namespace s2h::ecs

#endif // S2H_ENGINE_ECS_COMPONENT_CAMERA_H_