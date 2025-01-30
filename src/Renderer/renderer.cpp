#include "s2h/Renderer/renderer.h"
#include "s2h/Renderer/misc.h"
#include "s2h/Resource/texture.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <span>
#include <utility>
#include <vector>

namespace s2h
{
RendererBase::RendererBase(std::span<TextureDesc> descriptors)
{
  Initialize(descriptors);
}

void RendererBase::Clear(s2h::RenderTarget target, std::span<uint8_t> color)
{
  Texture& texture = renderTextures_[target.handle.id];
  uint8_t *pixels = texture.GetPixels();
  std::size_t n = Texture::CalculateBufferSize(texture.GetDescriptor());

  for (std::size_t i = 0; i < n; i += color.size())
  {
    std::ranges::copy(color, pixels + i);
  }
}

void RendererBase::Submit(s2h::RenderTarget target, uint8_t *pixels) noexcept
{
  std::copy_n(renderTextures_[target.handle.id].GetPixels(),
    Texture::CalculateBufferSize(
      renderTextures_[target.handle.id].GetDescriptor()),
    pixels);
}

std::vector<RenderTarget> RendererBase::Initialize(
  std::span<TextureDesc> descriptors)
{
  std::vector<RenderTarget> created;
  for (std::size_t i = 0; i < descriptors.size(); ++i)
  {
    TextureDesc& desc = descriptors[i];
    Texture texture = Texture(
      std::make_unique<uint8_t[]>(Texture::CalculateBufferSize(desc)), desc);
    if (renderTextures_.size() <= i)
    {
      renderTextures_.push_back(std::move(texture));
    }
    else
    {
      renderTextures_[0] = std::move(texture);
    }
    created.push_back(RenderTarget{.handle = i});
  }
  return created;
}

RenderTarget RendererBase::CreateRenderTexture(TextureDesc desc)
{
  renderTextures_.push_back(Texture(desc));
  return RenderTarget{
    .handle = (renderTextures_.size() - 1),
  };
}

void RendererBase::UpdateRenderTexture(RenderTarget target, TextureDesc desc)
{
  assert(target.handle.id <= renderTextures_.size());
  renderTextures_[target.handle.id] = Texture(desc);
}

RenderTarget RendererBase::AddRenderTexture(Texture&& texture)
{
  renderTextures_.push_back(std::move(texture));
  return RenderTarget{
    .handle = (renderTextures_.size() - 1),
  };
}

s2h::Texture& RendererBase::GetRenderTexture(RenderTarget target)
{
  assert(target.handle.id < renderTextures_.size());
  return renderTextures_[target.handle.id];
}
} // namespace s2h
