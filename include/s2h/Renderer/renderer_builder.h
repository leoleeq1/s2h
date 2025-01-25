#ifndef S2H_RENDERER_BUILDER_H_
#define S2H_RENDERER_BUILDER_H_

#include "nw/surface.h"

#include <memory>

namespace s2h
{
class RendererBase;

enum class RendererKind
{
  SoftwareRasterizer,
  SoftwareRaytracer,
  HardwareRasterizer,
  HardwareRaytracer,
};

class RendererBuilder
{
 public:
  explicit RendererBuilder() = default;
  ~RendererBuilder() = default;

  RendererBuilder& Set(s2h::RendererKind kind)
  {
    kind_ = kind;
    return *this;
  }

  RendererBuilder& Set(nw::Surface surface)
  {
    surface_ = surface;
    return *this;
  }

  std::unique_ptr<RendererBase> Build();

 private:
  RendererKind kind_;
  nw::Surface surface_;
};
} // namespace s2h

#endif // S2H_RENDERER_BUILDER_H_