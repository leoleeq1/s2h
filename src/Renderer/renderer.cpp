#include "s2h/Renderer/renderer.h"
#include "nw/surface.h"
#include "s2h/Renderer/descriptor.h"
#include "s2h/Renderer/texture.h"

#include <utility>

namespace s2h
{
RendererBase::RendererBase(const nw::Surface& surface)
{
  surface_ = surface;
}

void RendererBase::OnWindowSizeChanged(const nw::Surface& surface)
{
  surface_ = surface;
}

RenderTarget RendererBase::CreateAndAddRenderTexture(TextureDesc desc)
{
  renderTextures_.push_back(Texture(desc));
  return RenderTarget{
    .handle = static_cast<int32_t>(renderTextures_.size() - 1),
  };
}

RenderTarget RendererBase::AddRenderTexture(Texture&& texture)
{
  renderTextures_.push_back(std::move(texture));
  return RenderTarget{
    .handle = static_cast<int32_t>(renderTextures_.size() - 1),
  };
}
} // namespace s2h
