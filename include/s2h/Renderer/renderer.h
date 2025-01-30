#ifndef S2H_RENDERER_H_
#define S2H_RENDERER_H_

#include "s2h/Renderer/buffer.h"
#include "s2h/Renderer/misc.h"
#include "s2h/Resource/texture.h"

#include <span>
#include <vector>

namespace s2h
{
class RendererBase
{
 public:
  virtual ~RendererBase() {};

  std::vector<RenderTarget> Initialize(std::span<TextureDesc> descriptors);

  RenderTarget CreateRenderTexture(TextureDesc desc);
  void UpdateRenderTexture(RenderTarget target, TextureDesc desc);
  RenderTarget AddRenderTexture(Texture&& texture);
  s2h::Texture& GetRenderTexture(RenderTarget target);

  virtual void Clear(s2h::RenderTarget target, std::span<uint8_t> color);
  virtual void DrawIndexed(s2h::RenderTarget target,
    const s2h::ConstantBuffer& cb, const s2h::VertexBuffer& vb,
    const s2h::IndexBuffer& ib) = 0;
  void Submit(s2h::RenderTarget target, uint8_t *pixels) noexcept;

 protected:
  RendererBase() = default;
  RendererBase(std::span<TextureDesc> descriptors);

  std::vector<s2h::Texture> renderTextures_;
};
} // namespace s2h

#endif // S2H_RENDERER_H_