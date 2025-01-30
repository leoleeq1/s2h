#include "s2h/Renderer/texture.h"
#include "s2h/Renderer/misc.h"

#include <cstdint>
#include <memory>
#include <utility>

namespace s2h
{
Texture::Texture(TextureDesc desc)
{
  pixels_ = std::make_unique<uint8_t[]>(CalculateBufferSize(desc));
  desc_ = desc;
}

Texture::Texture(std::unique_ptr<uint8_t[]> pixels, TextureDesc desc)
{
  pixels_ = std::move(pixels);
  desc_ = desc;
}

Texture::Texture(Texture&& texture) noexcept
{
  pixels_ = std::move(texture.pixels_);
  texture.pixels_ = nullptr;
  desc_ = texture.desc_;
}

Texture& Texture::operator=(Texture&& texture) noexcept
{
  pixels_ = std::move(texture.pixels_);
  texture.pixels_ = nullptr;
  desc_ = texture.desc_;
  return *this;
}
} // namespace s2h
