#ifndef S2H_RENDERER_DESCRIPTOR_H_
#define S2H_RENDERER_DESCRIPTOR_H_

#include <cstddef>
#include <cstdint>

namespace s2h
{
template<std::size_t tag> struct Handle
{
  int32_t id;
};

using RenderTextureHandle = Handle<0>;

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

struct RenderTarget
{
  RenderTextureHandle handle;
};
} // namespace s2h

#endif // S2H_RENDERER_DESCRIPTOR_H_