#include "s2h/Renderer/texture.h"
#include "s2h/Renderer/descriptor.h"

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

constexpr std::size_t Texture::CalculateBufferSize(TextureDesc desc) noexcept
{
  int32_t bitCount = 0;
  switch (desc.format)
  {
    case TextureFormat::A8R8G8B8:
      bitCount = 32;
      break;
    case TextureFormat::D24S8:
      bitCount = 24;
      break;
    default:
      std::unreachable();
  }
  return static_cast<std::size_t>(
    (((desc.width * bitCount + 15) >> 4U) << 1U) * desc.height);
}

} // namespace s2h
