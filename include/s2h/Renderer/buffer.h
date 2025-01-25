#ifndef S2H_BUFFER_H_
#define S2H_BUFFER_H_

#include "s2h/Renderer/vertex.h"

#include <cassert>
#include <cstdint>
#include <vector>

namespace s2h
{
class ConstantBuffer
{
 public:
 private:
};

class VertexBuffer
{
 public:
  constexpr Vertex operator[](std::size_t i) const noexcept
  {
    assert(i < vertices.size());
    return vertices[i];
  }

 private:
  std::vector<Vertex> vertices;
};

class IndexBuffer
{
 public:
  constexpr const std::vector<std::size_t>& Get() const noexcept
  {
    return indices;
  }

 private:
  std::vector<std::size_t> indices;
};
} // namespace s2h

#endif S2H_BUFFER_H_