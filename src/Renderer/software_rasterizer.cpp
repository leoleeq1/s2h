#include "s2h/Renderer/software_rasterizer.h"
#include "nw/color.h"
#include "nw/surface.h"
#include "s2h/Math/math.h"
#include "s2h/Math/vector.h"
#include "s2h/buffer.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <print>

namespace s2h
{
void SoftwareRasterizer::Clear(nw::Color color)
{
  std::fill_n(surface_.pixels, surface_.Length(), color);
}

void SoftwareRasterizer::Draw()
{
  static float deg = 0;
  float rad = deg * s2h::numbers::fDeg2Rad;
  deg += 0.1f;
  float width = surface_.width * 0.5f;
  float height = surface_.height * 0.5f;
  float x = width + s2h::cos(rad) * 1000;
  float y = height + s2h::sin(rad) * 1000;
  DrawLine(v2i{x, y}, v2i{width, height}, nw::Color::black);
}

void SoftwareRasterizer::DrawIndexed(
  const s2h::VertexBuffer& vb, const s2h::IndexBuffer& ib)
{
  const std::vector<std::size_t> indices = ib.Get();
  for (std::size_t i = 0; i < indices.size() - 1; ++i)
  {
    // DrawLine(, , nw::Color::black);
  }
}

void SoftwareRasterizer::DrawLine(v2i v0, v2i v1, nw::Color color)
{
  v2i min{0, 0};
  v2i max{surface_.width, surface_.height};
  v2i half = (max - min) * 0.5;

  v0 -= half;
  v1 -= half;

  if (!CohenSutherlandClip(v0, v1, -half, half))
  {
    return;
  }

  v0 += half;
  v1 += half;

  std::println("v0: {}, {} / v1: {}, {}", v0[0], v0[1], v1[0], v1[1]);

  // Line drawing algorithm from
  // https://zingl.github.io/Bresenham.pdf
  int32_t xi = v0[0] < v1[0] ? 1 : -1;
  int32_t yi = v0[1] < v1[1] ? 1 : -1;
  int32_t dx = s2h::abs(v1[0] - v0[0]);
  int32_t dy = -s2h::abs(v1[1] - v0[1]);
  int32_t e = dx + dy;

  for (;;)
  {
    SetPixel(v0, color);
    int32_t e2 = 2 * e;
    if (e2 >= dy)
    {
      if (v0[0] == v1[0]) break;
      e += dy;
      v0[0] += xi;
    }

    if (e2 <= dx)
    {
      if (v0[1] == v1[1]) break;
      e += dx;
      v0[1] += yi;
    }
  }
}

bool SoftwareRasterizer::CohenSutherlandClip(v2i& v0, v2i& v1, v2i min, v2i max)
{
  auto GetRegion = [](v2i v, v2i min, v2i max) {
    int32_t region{0};
    if (v[0] < min[0])
    {
      region |= 0b0001;
    }
    if (v[0] > max[0])
    {
      region |= 0b0010;
    }
    if (v[1] < min[1])
    {
      region |= 0b0100;
    }
    if (v[1] > max[1])
    {
      region |= 0b1000;
    }
    return region;
  };

  auto r0 = GetRegion(v0, min, max);
  auto r1 = GetRegion(v1, min, max);

  if (r0 & r1)
  {
    return false;
  }

  float slope = static_cast<float>(v1[1] - v0[1]) / (v1[0] - v0[0]);
  float tmax = s2h::numbers::fRad2Deg
               * s2h::acos(s2h::dot(s2h::normalize(max), v2f::basis(0)));

  auto Clip = [&](v2i& v, int32_t r) {
    float t = s2h::numbers::fRad2Deg
              * s2h::acos(s2h::dot(s2h::normalize(v), v2f::basis(0)));

    if (t < tmax || t > (180.0f - tmax))
    {
      if (r & 0b0001)
      {
        v[1] = static_cast<int32_t>(slope * (min[0] - v0[0]) + v0[1]);
        v[0] = min[0];
      }
      else if (r & 0b0010)
      {
        v[1] = static_cast<int32_t>(slope * (max[0] - v0[0]) + v0[1]);
        v[0] = max[0];
      }
    }
    else
    {
      if (r & 0b0100)
      {
        v[0] = static_cast<int32_t>((min[1] - v0[1]) / slope + v0[0]);
        v[1] = min[1];
      }
      else if (r & 0b1000)
      {
        v[0] = static_cast<int32_t>((max[1] - v0[1]) / slope + v0[0]);
        v[1] = max[1];
      }
    }
  };

  Clip(v0, r0);
  Clip(v1, r1);

  return true;
}
} // namespace s2h