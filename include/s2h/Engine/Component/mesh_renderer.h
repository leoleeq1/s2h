#ifndef S2H_ENGINE_MESH_RENDERER_H_
#define S2H_ENGINE_MESH_RENDERER_H_

#include "s2h/Engine/Component/component.h"

namespace s2h
{
class Mesh;

class MeshRenderer : public Component
{
 public:
  void SetMesh(s2h::Mesh *mesh) { mesh_ = mesh; }
  s2h::Mesh *GetMesh() noexcept { return mesh_; }

 private:
  s2h::Mesh *mesh_;
};
} // namespace s2h

#endif // S2H_ENGINE_MESH_RENDERER_H_