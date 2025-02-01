#ifndef S2H_ENGINE_ECS_COMPONENT_MESH_RENDERER_H_
#define S2H_ENGINE_ECS_COMPONENT_MESH_RENDERER_H_

namespace s2h
{
class Mesh;

namespace ecs
{
struct MeshRendererComponent
{
  s2h::Mesh *mesh;
};
} // namespace ecs
} // namespace s2h

#endif // S2H_ENGINE_ECS_COMPONENT_MESH_RENDERER_H_