#include "s2h/Engine/ECS/System/transform.h"

#include "s2h/Math/matrix.h"
#include "s2h/Math/vector.h"

namespace s2h::ecs::TransformSystem
{
mat4 GetModelMatrix(s2h::v3f pos, s2h::v3f rot, s2h::v3f scale) noexcept
{
  mat4 t = GetTranslateMatrix(pos);
  mat4 r = GetRotationMatrix(rot);
  mat4 s = GetScaleMatrix(scale);
  return s * r * t;
}

mat4 GetTranslateMatrix(s2h::v3f position) noexcept
{
  return mat4{
    v4f::Basis(0),
    v4f::Basis(1),
    v4f::Basis(2),
    v4f{position[0], position[1], position[2], 1.0f},
  };
}

mat4 GetRotationMatrix(s2h::v3f rotation) noexcept
{
  float cp = s2h::cos(s2h::numbers::fDeg2Rad * rotation[0]);
  float sp = s2h::sin(s2h::numbers::fDeg2Rad * rotation[0]);
  float cy = s2h::cos(s2h::numbers::fDeg2Rad * rotation[1]);
  float sy = s2h::sin(s2h::numbers::fDeg2Rad * rotation[1]);
  float cr = s2h::cos(s2h::numbers::fDeg2Rad * rotation[2]);
  float sr = s2h::sin(s2h::numbers::fDeg2Rad * rotation[2]);

  return mat4{
    v4f{cr * cy + sr * sp * sy,  -sr * cy + cr * sp * sy, cp * sy, 0.0f},
    v4f{sr * cp,                 cr * cp,                 -sp,     0.0f},
    v4f{-sy * cr + sr * sp * cy, sr * sy + cr * sp * cy,  cp * cy, 0.0f},
    v4f::Basis(3),
  };
}

mat4 GetScaleMatrix(s2h::v3f scale) noexcept
{
  return mat4{
    v4f::Basis(0) * scale[0],
    v4f::Basis(1) * scale[1],
    v4f::Basis(2) * scale[2],
    v4f::Basis(3),
  };
}
} // namespace s2h::ecs::TransformSystem
