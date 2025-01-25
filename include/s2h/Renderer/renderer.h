#ifndef S2H_RENDERER_H_
#define S2H_RENDERER_H_

#include "nw/color.h"
#include "nw/surface.h"
#include "s2h/Renderer/descriptor.h"
#include "s2h/Renderer/texture.h"

#include <vector>

namespace s2h
{
class VertexBuffer;
class IndexBuffer;

class RendererBase
{
 public:
  virtual ~RendererBase() {};
  virtual void Clear(nw::Color color) = 0;
  virtual void Draw() = 0;
  virtual void DrawIndexed(
    const s2h::VertexBuffer& vb, const s2h::IndexBuffer& ib) = 0;

  void OnWindowSizeChanged(const nw::Surface& surface);
  RenderTarget CreateAndAddRenderTexture(TextureDesc desc);
  RenderTarget AddRenderTexture(Texture&& texture);

 protected:
  RendererBase() = default;
  RendererBase(const nw::Surface& surface);

  nw::Surface surface_;
  std::vector<s2h::Texture> renderTextures_;
};
} // namespace s2h

#endif // S2H_RENDERER_H_