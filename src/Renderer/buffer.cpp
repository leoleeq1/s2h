#include "s2h/Renderer/buffer.h"
#include "s2h/Renderer/vertex.h"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <span>
#include <vector>

namespace s2h
{
s2h::mat4 ConstantBuffer::GetModelMatrix() const noexcept
{
  return modelMatrix_;
}

s2h::mat4 ConstantBuffer::GetViewMatrix() const noexcept
{
  return viewMatrix_;
}

s2h::mat4 ConstantBuffer::GetProjectionMatrix() const noexcept
{
  return projectionMatrix_;
}

void ConstantBuffer::SetModelMatrix(s2h::mat4 matrix) noexcept
{
  modelMatrix_ = matrix;
}

void ConstantBuffer::SetViewMatrix(s2h::mat4 matrix) noexcept
{
  viewMatrix_ = matrix;
}

void ConstantBuffer::SetProjectionMatrix(s2h::mat4 matrix) noexcept
{
  projectionMatrix_ = matrix;
}
} // namespace s2h
