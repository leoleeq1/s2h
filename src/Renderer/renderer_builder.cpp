#include "s2h/Renderer/renderer_builder.h"
#include "s2h/Renderer/hardware_rasterizer.h"
#include "s2h/Renderer/renderer.h"
#include "s2h/Renderer/software_rasterizer.h"

#include <memory>
#include <utility>

namespace s2h
{
std::unique_ptr<RendererBase> RendererBuilder::Build()
{
  switch (kind_)
  {
    case RendererKind::SoftwareRasterizer:
      return std::make_unique<SoftwareRasterizer>();
    case RendererKind::SoftwareRaytracer:
      [[fallthrough]];
    case RendererKind::HardwareRasterizer:
      [[fallthrough]];
    case RendererKind::HardwareRaytracer:
      [[fallthrough]];
    default:
      std::unreachable();
  }
}

} // namespace s2h
