#ifndef S2H_RENDERER_DESCRIPTOR_H_
#define S2H_RENDERER_DESCRIPTOR_H_

#include <cstddef>
#include <cstdint>

namespace s2h
{
template<std::size_t tag> struct Handle
{
  std::size_t id;
};

using RenderTextureHandle = Handle<0>;
struct RenderTarget
{
  RenderTextureHandle handle;
};
} // namespace s2h

#endif // S2H_RENDERER_DESCRIPTOR_H_