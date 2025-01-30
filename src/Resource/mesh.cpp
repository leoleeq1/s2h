#include "s2h/Resource/mesh.h"
#include "s2h/IO/reader.h"
#include "s2h/IO/serialize.h"
#include "s2h/IO/writer.h"
#include "s2h/Renderer/buffer.h"
#include "s2h/Renderer/vertex.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace s2h
{
Mesh::Mesh(
  const std::vector<Vertex>& vertices, const std::vector<std::size_t>& indices)
{
  vb_ = vertices;
  ib_ = indices;
}

Mesh::Mesh(std::span<const Vertex> vertices, std::span<const int32_t> indices)
{
  vb_.resize(vertices.size());
  ib_.resize(indices.size());
  std::ranges::copy(vertices, vb_.begin());
  for (std::size_t i = 0; i < indices.size(); ++i)
  {
    ib_[i] = static_cast<std::size_t>(indices[i]);
  }
}

Mesh::Mesh(
  std::span<const Vertex> vertices, std::span<const std::size_t> indices)
{
  vb_.resize(vertices.size());
  ib_.resize(indices.size());
  std::ranges::copy(vertices, vb_.begin());
  std::ranges::copy(indices, ib_.begin());
}

void Mesh::Serialize(IWriter& writer) const
{
  std::size_t vertCount = vb_.size();
  writer.WriteUInt64(vertCount);

  for (std::size_t i = 0; i < vertCount; ++i)
  {
    const Vertex& vert = vb_[i];
    writer.WriteFloat(vert.position[0]);
    writer.WriteFloat(vert.position[1]);
    writer.WriteFloat(vert.position[2]);
  }

  std::size_t indexCount = ib_.size();
  writer.WriteUInt64(indexCount);

  for (std::size_t i = 0; i < indexCount; ++i)
  {
    writer.WriteUInt64(ib_[i]);
  }
}

void Mesh::Deserialize(IReader& reader)
{
  std::size_t vertCount{};
  reader.ReadUInt64(&vertCount);

  vb_.clear();
  vb_.reserve(vertCount);

  for (std::size_t i = 0; i < vertCount; ++i)
  {
    Vertex v{};
    reader.ReadFloat(&v.position[0]);
    reader.ReadFloat(&v.position[1]);
    reader.ReadFloat(&v.position[2]);
    vb_.push_back(v);
  }

  std::size_t indexCount{};
  reader.ReadUInt64(&indexCount);

  ib_.clear();
  ib_.reserve(indexCount);

  for (std::size_t i = 0; i < indexCount; ++i)
  {
    std::size_t index{};
    reader.ReadUInt64(&index);
    ib_.push_back(index);
  }
}
} // namespace s2h
