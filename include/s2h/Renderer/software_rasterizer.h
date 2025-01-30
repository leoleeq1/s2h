#ifndef S2H_SOFTWARE_RASTERIZER_H_
#define S2H_SOFTWARE_RASTERIZER_H_

#include "nw/color.h"
#include "s2h/Math/vector.h"
#include "s2h/Renderer/buffer.h"
#include "s2h/Renderer/renderer.h"
#include "s2h/Resource/texture.h"

namespace s2h
{
class SoftwareRasterizer final : public RendererBase
{
 public:
  SoftwareRasterizer() = default;
  virtual ~SoftwareRasterizer() = default;
  void DrawIndexed(s2h::RenderTarget target, const s2h::ConstantBuffer& cb,
    const s2h::VertexBuffer& vb, const s2h::IndexBuffer& ib) override;

 private:
  inline void SetPixel(s2h::RenderTarget target, s2h::v2i v, nw::Color color);
  void DrawLine(
    s2h::RenderTarget target, s2h::v2f v0, s2h::v2f v1, nw::Color color);
  bool CohenSutherlandClip(
    s2h::v2f& v0, s2h::v2f& v1, s2h::v2f min, s2h::v2f max);
};

inline void SoftwareRasterizer::SetPixel(
  s2h::RenderTarget target, s2h::v2i v, nw::Color color)
{
  if (v[0] < 0 || v[0] >= renderTextures_[target.handle.id].GetWidth()
      || v[1] < 0 || v[1] >= renderTextures_[target.handle.id].GetHeight())
  {
    return;
  }
  nw::Color *pixels = reinterpret_cast<nw::Color *>(
    renderTextures_[target.handle.id].GetPixels());
  pixels[v[1] * renderTextures_[target.handle.id].GetWidth() + v[0]] = color;
}
} // namespace s2h

#endif S2H_SOFTWARE_RASTERIZER_H_