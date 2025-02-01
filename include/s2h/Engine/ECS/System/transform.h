#ifndef S2H_ENGINE_ECS_SYSTEM_TRANSFORM_H_
#define S2H_ENGINE_ECS_SYSTEM_TRANSFORM_H_

#include "s2h/Math/matrix.h"
#include "s2h/Math/vector.h"

namespace s2h::ecs::TransformSystem
{
mat4 GetModelMatrix(s2h::v3f pos, s2h::v3f rot, s2h::v3f scale) noexcept;
mat4 GetTranslateMatrix(s2h::v3f position) noexcept;
mat4 GetRotationMatrix(s2h::v3f rotation) noexcept;
mat4 GetScaleMatrix(s2h::v3f scale) noexcept;
} // namespace s2h::ecs::TransformSystem

#endif // S2H_ENGINE_ECS_SYSTEM_TRANSFORM_H_