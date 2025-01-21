#ifndef S2H_VECTOR_H_
#define S2H_VECTOR_H_

#include "math.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <span>
#include <type_traits>
#include <utility>

namespace s2h
{
template<typename T>
concept Arithmetic = std::is_integral_v<T> || std::is_floating_point_v<T>;

template<typename T, std::size_t N>
  requires Arithmetic<T>
struct vec
{
  vec() : v{} {}
  vec(const vec& rhs) : v{rhs.v} {}
  template<typename... E>
  explicit vec(E&&...e)
    requires(!std::is_same_v<vec, std::decay_t<E>> && ...)
    : v{std::forward<E>(e)...}
  {
  }
  explicit vec(std::span<T, N> s) : v{s} {}

  T& operator[](size_t i)
  {
    assert(i >= 0 && i < N);
    return v[i];
  }

  std::array<T, N> v;
};

template<typename T>
  requires Arithmetic<T>
struct alignas(alignof(T) * 4) vec<T, 4>
{
  vec() : v{} {}
  vec(const vec& rhs) : v{rhs.v} {}
  template<typename... E>
  explicit vec(E&&...e)
    requires(!std::is_same_v<vec, std::decay_t<E>> && ...)
    : v{std::forward<E>(e)...}
  {
  }
  explicit vec(std::span<T, 4> s) : v{s} {}

  T& operator[](size_t i)
  {
    assert(i >= 0 && i < 4);
    return v[i];
  }

  std::array<T, 4> v;
};

using v2f = vec<float, 2>;
using v3f = vec<float, 3>;
using v4f = vec<float, 4>;

using v2i = vec<int32_t, 2>;
using v3i = vec<int32_t, 3>;
using v4i = vec<int32_t, 4>;

template<typename T, std::size_t N>
  requires std::is_integral_v<T>
bool operator==(vec<T, N> lhs, vec<T, N> rhs)
{
  for (std::size_t i = 0; i < N; ++i)
  {
    if (lhs[i] != rhs[i])
    {
      return false;
    }
  }
  return true;
}

template<typename T, std::size_t N>
  requires std::is_integral_v<T>
bool operator!=(vec<T, N> lhs, vec<T, N> rhs)
{
  return !operator==(lhs, rhs);
}

template<typename T, std::size_t N>
  requires std::is_floating_point_v<T>
bool equals(vec<T, N> lhs, vec<T, N> rhs,
  T epsilon = std::numeric_limits<T>::epsilon(), int32_t maxUlpDiff = 4)
{
  assert(maxUlpDiff > 0 && maxUlpDiff <= 4);
  for (std::size_t i = 0; i < N; ++i)
  {
    if (!equals(lhs[i], rhs[i], epsilon, maxUlpDiff))
    {
      return false;
    }
  }

  return true;
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
vec<T, N> operator+(vec<T, N> lhs, vec<T, N> rhs)
{
  for (std::size_t i = 0; i < N; ++i)
  {
    lhs[i] += rhs[i];
  }
  return lhs;
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
vec<T, N> operator-(vec<T, N> lhs, vec<T, N> rhs)
{
  for (std::size_t i = 0; i < N; ++i)
  {
    lhs[i] -= rhs[i];
  }
  return lhs;
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
vec<T, N> operator-(vec<T, N> v)
{
  for (std::size_t i = 0; i < N; ++i)
  {
    v[i] = -v[i];
  }
  return v;
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
vec<T, N> operator*(Arithmetic auto lhs, vec<T, N> rhs)
{
  for (std::size_t i = 0; i < N; ++i)
  {
    rhs[i] = lhs * rhs[i];
  }
  return rhs;
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
vec<T, N> operator*(vec<T, N> lhs, Arithmetic auto rhs)
{
  for (std::size_t i = 0; i < N; ++i)
  {
    lhs[i] *= rhs;
  }
  return lhs;
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
vec<T, N> operator/(vec<T, N> lhs, Arithmetic auto rhs)
{
  assert(rhs > std::numeric_limits<decltype(rhs)>::epsilon()
         || rhs < -std::numeric_limits<decltype(rhs)>::epsilon());
  for (std::size_t i = 0; i < N; ++i)
  {
    lhs[i] /= rhs;
  }
  return lhs;
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
T dot(vec<T, N> lhs, vec<T, N> rhs)
{
  T result{};
  for (std::size_t i = 0; i < N; ++i)
  {
    result += lhs[i] * rhs[i];
  }
  return result;
}

template<typename T>
  requires Arithmetic<T>
T cross(vec<T, 2> lhs, vec<T, 2> rhs)
{
  return (lhs[0] * rhs[1]) - (lhs[1] * rhs[0]);
}

template<typename T>
  requires Arithmetic<T>
vec<T, 3> cross(vec<T, 3> lhs, vec<T, 3> rhs)
{
  return vec<T, 3>{(lhs[1] * rhs[2]) - (lhs[2] * rhs[1]),
    -((lhs[0] * rhs[2]) - (lhs[2] * rhs[0])),
    (lhs[0] * rhs[1]) - (lhs[1] * rhs[0])};
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
auto length(vec<T, N> v)
{
  return sqrt(dot(v, v));
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
T sqrLength(vec<T, N> v)
{
  return dot(v, v);
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
vec<T, N> normalize(vec<T, N> v)
{
  auto l = length(v);
  return v / l;
}
} // namespace s2h
#endif // S2H_VECTOR_H_