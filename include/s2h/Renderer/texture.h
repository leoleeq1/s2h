#ifndef S2H_TEXTURE_H_
#define S2H_TEXTURE_H_

#include <cstdint>
#include <memory>

namespace s2h
{
enum class TextureFormat
{
  None = 0,
  A8R8G8B8,
  D24S8,
};

struct TextureDesc
{
  TextureFormat format = TextureFormat::None;
  int32_t width = 0;
  int32_t height = 0;
};

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

  [[nodiscard]] TextureDesc GetDescriptor() const noexcept { return desc_; }
  [[nodiscard]] int32_t GetWidth() const noexcept { return desc_.width; }
  [[nodiscard]] int32_t GetHeight() const noexcept { return desc_.height; }
  [[nodiscard]] uint8_t *GetPixels() const noexcept { return pixels_.get(); }

  [[nodiscard]] static constexpr std::size_t CalculateBufferSize(
    TextureDesc desc) noexcept;

 private:
  std::unique_ptr<uint8_t[]> pixels_;
  TextureDesc desc_;
};

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

#endif // S2H_TEXTURE_H_