#include "s2h/Renderer/software_rasterizer.h"
#include "nw/color.h"
#include "nw/surface.h"
#include "s2h/Math/math.h"
#include "s2h/Math/vector.h"
#include "s2h/Renderer/buffer.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <print>

namespace s2h
{
void SoftwareRasterizer::DrawIndexed(s2h::RenderTarget target,
  const s2h::ConstantBuffer& cb, const s2h::VertexBuffer& vb,
  const s2h::IndexBuffer& ib)
{
  const std::size_t n = ib.size();
  for (std::size_t i = 0; i < n - 1; ++i)
  {
    s2h::v4f p0 = v4f(vb[ib[i]].position, 1.0f) * cb.GetModelMatrix()
                  * cb.GetViewMatrix() * cb.GetProjectionMatrix();
    s2h::v4f p1 = v4f(vb[ib[i + 1]].position, 1.0f) * cb.GetModelMatrix()
                  * cb.GetViewMatrix() * cb.GetProjectionMatrix();

    if (s2h::isNearlyZero(p0[3]) || s2h::isNearlyZero(p1[3])) continue;

    p0 = p0 / p0[3];
    p1 = p1 / p1[3];

    DrawLine(target, p0, p1, nw::Color::black);
  }
}

void SoftwareRasterizer::DrawLine(
  s2h::RenderTarget target, v2f v0, v2f v1, nw::Color color)
{
  if (!CohenSutherlandClip(v0, v1, -v2f::One(), v2f::One()))
  {
    return;
  }

  v2i screenSize = v2i{renderTextures_[target.handle.id].GetWidth(),
    renderTextures_[target.handle.id].GetHeight()};
  v2f half = screenSize * 0.5f;

  v0 = v0 * half + half;
  v1 = v1 * half + half;

  // Line drawing algorithm from
  // https://zingl.github.io/Bresenham.pdf
  int32_t xi = v0[0] < v1[0] ? 1 : -1;
  int32_t yi = v0[1] < v1[1] ? 1 : -1;
  int32_t dx = static_cast<int32_t>(s2h::abs(v1[0] - v0[0]));
  int32_t dy = static_cast<int32_t>(-s2h::abs(v1[1] - v0[1]));
  int32_t e = dx + dy;
  v2i cur = v0;
  v2i end = v1;

  for (;;)
  {
    SetPixel(target, cur, color);
    int32_t e2 = 2 * e;
    if (e2 >= dy)
    {
      if (cur[0] == end[0]) break;
      e += dy;
      cur[0] += xi;
    }

    if (e2 <= dx)
    {
      if (cur[1] == end[1]) break;
      e += dx;
      cur[1] += yi;
    }
  }
}

bool SoftwareRasterizer::CohenSutherlandClip(v2f& v0, v2f& v1, v2f min, v2f max)
{
  auto GetRegion = [](v2f v, v2f min, v2f max) {
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
               * s2h::acos(s2h::dot(s2h::normalize(max), v2f::Basis(0)));

  auto Clip = [&](v2f& v, int32_t r) {
    float t = s2h::numbers::fRad2Deg
              * s2h::acos(s2h::dot(s2h::normalize(v), v2f::Basis(0)));

    if (t < tmax || t > (180.0f - tmax))
    {
      if (r & 0b0001)
      {
        v[1] = slope * (min[0] - v0[0]) + v0[1];
        v[0] = min[0];
      }
      else if (r & 0b0010)
      {
        v[1] = slope * (max[0] - v0[0]) + v0[1];
        v[0] = max[0];
      }
    }
    else
    {
      if (r & 0b0100)
      {
        v[0] = (min[1] - v0[1]) / slope + v0[0];
        v[1] = min[1];
      }
      else if (r & 0b1000)
      {
        v[0] = (max[1] - v0[1]) / slope + v0[0];
        v[1] = max[1];
      }
    }
  };

  Clip(v0, r0);
  Clip(v1, r1);

  return true;
}
} // namespace s2h