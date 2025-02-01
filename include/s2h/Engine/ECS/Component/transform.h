#ifndef S2H_ENGINE_ECS_COMPONENT_TRANSFORM_H_
#define S2H_ENGINE_ECS_COMPONENT_TRANSFORM_H_

#include "s2h/Math/vector.h"

namespace s2h::ecs
{
struct PositionComponent
{
  s2h::v3f position{};
};

struct RotationComponent
{
  s2h::v3f rotation{};
};

struct ScaleComponent
{
  s2h::v3f scale{1.0f, 1.0f, 1.0f};
};
} // namespace s2h::ecs

#endif // S2H_ENGINE_ECS_COMPONENT_TRANSFORM_H_