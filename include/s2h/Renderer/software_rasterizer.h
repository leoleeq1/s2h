#ifndef S2H_SOFTWARE_RASTERIZER_H_
#define S2H_SOFTWARE_RASTERIZER_H_

#include "nw/surface.h"
#include "s2h/Math/vector.h"
#include "s2h/Renderer/renderer.h"

namespace s2h
{
class SoftwareRasterizer final : public RendererBase
{
 public:
  SoftwareRasterizer(const nw::Surface& surface) : RendererBase{surface} {}
  virtual ~SoftwareRasterizer() = default;
  void Clear(nw::Color color) override;
  void Draw() override;
  void DrawIndexed(
    const s2h::VertexBuffer& vb, const s2h::IndexBuffer& ib) override;

 private:
  inline void SetPixel(s2h::v2i v, nw::Color color);
  void DrawLine(s2h::v2i v0, s2h::v2i v1, nw::Color color);
  bool CohenSutherlandClip(
    s2h::v2i& v0, s2h::v2i& v1, s2h::v2i min, s2h::v2i max);
};

inline void SoftwareRasterizer::SetPixel(s2h::v2i v, nw::Color color)
{
  if (v[0] < 0 || v[0] >= surface_.width || v[1] < 0 || v[1] >= surface_.height)
  {
    return;
  }
  surface_.pixels[v[1] * surface_.width + v[0]] = color;
}
} // namespace s2h

#endif S2H_SOFTWARE_RASTERIZER_H_