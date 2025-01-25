#ifndef S2H_MATH_VECTOR_H_
#define S2H_MATH_VECTOR_H_

#include "s2h/Math/math.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <span>
#include <type_traits>
#include <utility>

namespace s2h
{
template<typename T, std::size_t N>
  requires Arithmetic<T>
struct vec
{
  constexpr vec() : v{} {}
  constexpr vec(const vec& rhs) : v{rhs.v} {}
  explicit constexpr vec(std::span<T, N> s) : v{s} {}

  template<s2h::Arithmetic U> explicit constexpr vec(const vec<U, N>& rhs)
  {
    for (std::size_t i = 0; i < N; ++i)
    {
      v[i] = static_cast<T>(rhs.v[i]);
    }
  }

  template<typename... E>
  explicit constexpr vec(E&&...e)
    requires(
      sizeof...(E) == N && (std::is_arithmetic_v<std::decay_t<E>> && ...))
    : v{static_cast<T>(e)...}
  {
  }

  static consteval vec basis(std::size_t i)
  {
    vec<T, N> temp;
    for (std::size_t j = 0; j < N; ++j)
    {
      temp.v[j] = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
    }
    return temp;
  }

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
  constexpr vec() : v{} {}
  constexpr vec(const vec& rhs) : v{rhs.v} {}
  explicit constexpr vec(std::span<T, 4> s) : v{s} {}

  template<s2h::Arithmetic U> explicit constexpr vec(const vec<U, 4>& rhs)
  {
    for (std::size_t i = 0; i < 4; ++i)
    {
      v[i] = static_cast<T>(rhs.v[i]);
    }
  }

  template<typename... E>
  explicit constexpr vec(E&&...e)
    requires(
      sizeof...(E) == 4 && (std::is_arithmetic_v<std::decay_t<E>> && ...))
    : v{static_cast<T>(e)...}
  {
  }

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
constexpr bool operator==(vec<T, N> lhs, vec<T, N> rhs)
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
constexpr bool operator!=(vec<T, N> lhs, vec<T, N> rhs)
{
  return !operator==(lhs, rhs);
}

template<typename T, std::size_t N>
  requires std::is_floating_point_v<T>
constexpr bool equals(vec<T, N> lhs, vec<T, N> rhs,
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
constexpr vec<T, N> operator+(vec<T, N> lhs, vec<T, N> rhs)
{
  for (std::size_t i = 0; i < N; ++i)
  {
    lhs[i] += rhs[i];
  }
  return lhs;
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
constexpr vec<T, N>& operator+=(vec<T, N>& lhs, vec<T, N> rhs)
{
  for (std::size_t i = 0; i < N; ++i)
  {
    lhs[i] += rhs[i];
  }
  return lhs;
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
constexpr vec<T, N> operator-(vec<T, N> lhs, vec<T, N> rhs)
{
  for (std::size_t i = 0; i < N; ++i)
  {
    lhs[i] -= rhs[i];
  }
  return lhs;
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
constexpr vec<T, N>& operator-=(vec<T, N>& lhs, vec<T, N> rhs)
{
  for (std::size_t i = 0; i < N; ++i)
  {
    lhs[i] -= rhs[i];
  }
  return lhs;
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
constexpr vec<T, N> operator-(vec<T, N> v)
{
  for (std::size_t i = 0; i < N; ++i)
  {
    v[i] = -v[i];
  }
  return v;
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
constexpr vec<T, N> operator*(Arithmetic auto lhs, vec<T, N> rhs)
{
  for (std::size_t i = 0; i < N; ++i)
  {
    rhs[i] = lhs * rhs[i];
  }
  return rhs;
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
constexpr vec<T, N> operator*(vec<T, N> lhs, Arithmetic auto rhs)
{
  for (std::size_t i = 0; i < N; ++i)
  {
    lhs[i] = static_cast<T>(lhs[i] * rhs);
  }
  return lhs;
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
constexpr vec<T, N> operator/(vec<T, N> lhs, Arithmetic auto rhs)
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
constexpr T dot(vec<T, N> lhs, vec<T, N> rhs)
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
constexpr T cross(vec<T, 2> lhs, vec<T, 2> rhs)
{
  return (lhs[0] * rhs[1]) - (lhs[1] * rhs[0]);
}

template<typename T>
  requires Arithmetic<T>
constexpr vec<T, 3> cross(vec<T, 3> lhs, vec<T, 3> rhs)
{
  return vec<T, 3>{(lhs[1] * rhs[2]) - (lhs[2] * rhs[1]),
    -((lhs[0] * rhs[2]) - (lhs[2] * rhs[0])),
    (lhs[0] * rhs[1]) - (lhs[1] * rhs[0])};
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
constexpr auto length(vec<T, N> v)
{
  return static_cast<s2h::same_size_float<T>::type>(s2h::sqrt(dot(v, v)));
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
constexpr T sqrLength(vec<T, N> v)
{
  return dot(v, v);
}

template<typename T, std::size_t N>
  requires Arithmetic<T>
constexpr vec<s2h::same_size_float_t<T>, N> normalize(vec<T, N> v)
{
  auto l = length(v);
  if (s2h::isNearlyZero(l)) return vec<s2h::same_size_float_t<T>, N>{};
  return vec<s2h::same_size_float_t<T>, N>(v) / l;
}
} // namespace s2h
#endif // S2H_MATH_VECTOR_H_