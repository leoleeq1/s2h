#ifndef S2H_RENDERER_H_
#define S2H_RENDERER_H_

#include "nw/color.h"

namespace nw
{
struct Surface;
} // namespace nw

namespace s2h
{
class VertexBuffer;
class IndexBuffer;

class IRenderer
{
 public:
  virtual ~IRenderer() {};
  virtual void Clear(nw::Color color) = 0;
  virtual void Draw() = 0;
  virtual void DrawIndexed(
    const s2h::VertexBuffer& vb, const s2h::IndexBuffer& ib) = 0;

 protected:
  IRenderer() = default;
};
} // namespace s2h

#endif // S2H_RENDERER_H_