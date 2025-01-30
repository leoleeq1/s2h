#ifndef S2H_RENDERER_SHADER_H_
#define S2H_RENDERER_SHADER_H_

#include <vector>

namespace s2h
{
class Texture;

template<typename CB, typename InVert, typename OutVert> class VertexShader
{
 public:
  std::vector<OutVert> Run(const CB& cb, std::span<const InVert> verts) = 0;
};

template<typename CB, typename InVert> class FragmentShader
{
 public:
  void Run(Texture *target, const CB& cb, std::span<const InVert> verts) = 0;
};
} // namespace s2h

#endif // S2H_RENDERER_SHADER_H_