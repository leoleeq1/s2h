#ifndef S2H_BUFFER_H_
#define S2H_BUFFER_H_

#include "s2h/Math/matrix.h"
#include "s2h/Renderer/vertex.h"

#include <cassert>
#include <cstdint>
#include <span>
#include <vector>

namespace s2h
{
using VertexBuffer = std::vector<s2h::Vertex>;
using IndexBuffer = std::vector<std::size_t>;

class ConstantBuffer
{
 public:
  s2h::mat4 GetModelMatrix() const noexcept;
  s2h::mat4 GetViewMatrix() const noexcept;
  s2h::mat4 GetProjectionMatrix() const noexcept;

  void SetModelMatrix(const s2h::mat4& matrix) noexcept;
  void SetViewMatrix(const s2h::mat4& matrix) noexcept;
  void SetProjectionMatrix(const s2h::mat4& matrix) noexcept;

 private:
  s2h::mat4 modelMatrix_;
  s2h::mat4 viewMatrix_;
  s2h::mat4 projectionMatrix_;
};
} // namespace s2h

#endif S2H_BUFFER_H_