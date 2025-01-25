#ifndef S2H_TEXTURE_H_
#define S2H_TEXTURE_H_

#include "descriptor.h"

#include <cstdint>
#include <memory>

namespace s2h
{
class Texture
{
 public:
  explicit Texture(TextureDesc desc);
  explicit Texture(std::unique_ptr<uint8_t[]> pixels, TextureDesc desc);
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;
  Texture(Texture&& texture) noexcept;
  Texture& operator=(Texture&& texture) noexcept;

  [[nodiscard]] TextureFormat GetTextureFormat() const noexcept
  {
    return desc_.format;
  }

  [[nodiscard]] int32_t GetWidth() const noexcept { return desc_.width; }
  [[nodiscard]] int32_t GetHeight() const noexcept { return desc_.height; }

  [[nodiscard]] static constexpr std::size_t CalculateBufferSize(
    TextureDesc desc) noexcept;

 private:
  std::unique_ptr<uint8_t[]> pixels_;
  TextureDesc desc_;
};
} // namespace s2h

#endif // S2H_TEXTURE_H_