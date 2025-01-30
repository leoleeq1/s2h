#ifndef S2H_RESOURCE_MESH_H_
#define S2H_RESOURCE_MESH_H_

#include "s2h/IO/serialize.h"
#include "s2h/Renderer/buffer.h"
#include "s2h/Renderer/vertex.h"
#include "s2h/Resource/resource.h"

#include <cstddef>
#include <span>
#include <vector>

namespace s2h
{
class IWriter;
class IReader;
class Mesh : public IResource, public ISerializable<Mesh>
{
 public:
  Mesh(const std::vector<Vertex>& vertices,
    const std::vector<std::size_t>& indices);
  Mesh(std::span<const Vertex> vertices, std::span<const int32_t> indices);
  Mesh(std::span<const Vertex> vertices, std::span<const std::size_t> indices);
  virtual ~Mesh() = default;

  [[nodiscard]] const VertexBuffer& GetVB() const noexcept { return vb_; }
  [[nodiscard]] const IndexBuffer& GetIB() const noexcept { return ib_; }

  void Serialize(IWriter& writer) const override;
  void Deserialize(IReader& reader) override;

 private:
  VertexBuffer vb_;
  IndexBuffer ib_;
};
} // namespace s2h

#endif // S2H_RESOURCE_MESH_H_