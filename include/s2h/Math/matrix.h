#ifndef S2H_MATH_MATRIX_H_
#define S2H_MATH_MATRIX_H_

#include "s2h/Math/math.h"
#include "s2h/Math/vector.h"

#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <span>
#include <type_traits>

namespace s2h
{
template<typename T, std::size_t N>
  requires std::is_floating_point_v<T>
struct mat
{
  constexpr mat() : m{} {}
  constexpr mat(const mat& m) : m{m.m} {}
  constexpr explicit mat(std::span<T, N * N> s)
  {
    for (std::size_t r = 0; r < N; ++r)
    {
      for (std::size_t c = 0; c < N; ++c)
      {
        m[r][c] = s[r * N + c];
      }
    }
  }

  constexpr explicit mat(std::span<s2h::vec<T, N>, N> sv)
  {
    for (std::size_t r = 0; r < N; ++r)
    {
      for (std::size_t c = 0; c < N; ++c)
      {
        m[r][c] = sv[r][c];
      }
    }
  }

  constexpr explicit mat(std::initializer_list<s2h::vec<T, N>> list)
  {
    assert(list.size() == N);
    for (std::size_t r = 0; r < N; ++r)
    {
      m[r] = list.begin()[r];
    }
  }

  template<Arithmetic U> constexpr explicit mat(std::initializer_list<U> list)
  {
    assert(list.size() == (N * N));
    for (std::size_t r = 0; r < N; ++r)
    {
      for (std::size_t c = 0; c < N; ++c)
      {
        m[r][c] = static_cast<T>(list.begin()[r * N + c]);
      }
    }
  }

  static consteval mat Identity() noexcept
  {
    mat m{};
    for (std::size_t r = 0; r < N; ++r)
    {
      for (std::size_t c = 0; c < N; ++c)
      {
        m[r][c] = (r == c) ? static_cast<T>(1) : static_cast<T>(0);
      }
    }
    return m;
  }

  s2h::vec<T, N>& operator[](std::size_t row) { return m[row]; }

  std::array<s2h::vec<T, N>, N> m;
};

template<typename T, std::size_t Row, std::size_t Col> struct mat_nm
{
  constexpr mat_nm() : m{} {}
  constexpr mat_nm(const mat_nm& m) : m{m.m} {}
  constexpr explicit mat_nm(std::span<T, Row * Col> s)
  {
    for (std::size_t r = 0; r < Row; ++r)
    {
      for (std::size_t c = 0; c < Col; ++c)
      {
        m[r][c] = s[r * Col + c];
      }
    }
  }

  constexpr explicit mat_nm(std::span<s2h::vec<T, Col>, Row> sv)
  {
    for (std::size_t r = 0; r < Row; ++r)
    {
      for (std::size_t c = 0; c < Col; ++c)
      {
        m[r][c] = sv[r][c];
      }
    }
  }

  template<Arithmetic U>
  constexpr explicit mat_nm(std::initializer_list<U> list)
  {
    assert(list.size() == (Row * Col));
    for (std::size_t r = 0; r < Row; ++r)
    {
      for (std::size_t c = 0; c < Col; ++c)
      {
        m[r][c] = static_cast<T>(list.begin()[r * Col + c]);
      }
    }
  }

  s2h::vec<T, Col>& operator[](std::size_t row) { return m[row]; }

  std::array<vec<T, Col>, Row> m;
};

using mat2 = mat<float, 2>;
using mat3 = mat<float, 3>;
using mat4 = mat<float, 4>;

template<typename T, std::size_t N>
constexpr s2h::mat<T, N> Transpose(s2h::mat<T, N> m)
{
  for (std::size_t r = 1; r < N; ++r)
  {
    for (std::size_t c = 0; c < r; ++c)
    {
      std::swap(m[r][c], m[c][r]);
    }
  }
  return m;
}

template<typename T, std::size_t N>
constexpr s2h::mat<T, N> operator+(s2h::mat<T, N> lhs, s2h::mat<T, N> rhs)
{
  for (std::size_t r = 0; r < N; ++r)
  {
    for (std::size_t c = 0; c < N; ++c)
    {
      lhs[r][c] += rhs[r][c];
    }
  }
  return lhs;
}

template<typename T, std::size_t N>
constexpr s2h::mat<T, N> operator-(s2h::mat<T, N> lhs, s2h::mat<T, N> rhs)
{
  for (std::size_t r = 0; r < N; ++r)
  {
    for (std::size_t c = 0; c < N; ++c)
    {
      lhs[r][c] -= rhs[r][c];
    }
  }
  return lhs;
}

template<typename T, std::size_t N>
constexpr s2h::mat<T, N> operator*(s2h::mat<T, N> lhs, s2h::mat<T, N> rhs)
{
  s2h::mat<T, N> m{};
  rhs = s2h::Transpose(rhs);
  for (std::size_t r = 0; r < N; ++r)
  {
    for (std::size_t q = 0; q < N; ++q)
    {
      m[r][q] = s2h::dot(lhs[r] * rhs[q], s2h::vec<T, N>::One());
    }
  }
  return m;
}

template<typename T, std::size_t N>
constexpr s2h::vec<T, N> operator*(s2h::vec<T, N> lhs, s2h::mat<T, N> rhs)
{
  s2h::vec<T, N> v{};
  rhs = s2h::Transpose(rhs);
  for (std::size_t r = 0; r < N; ++r)
  {
    v[r] = s2h::dot(lhs * rhs[r], s2h::vec<T, N>::One());
  }
  return v;
}

template<typename T, std::size_t N>
constexpr s2h::mat<T, N> operator/(s2h::mat<T, N> lhs, Arithmetic auto rhs)
{
  for (std::size_t r = 0; r < N; ++r)
  {
    lhs[r] /= rhs;
  }
  return lhs;
}

template<typename T, std::size_t N>
constexpr s2h::mat<T, N> operator/(Arithmetic auto lhs, s2h::mat<T, N> rhs)
{
  for (std::size_t r = 0; r < N; ++r)
  {
    rhs[r] = lhs / rhs[r];
  }
  return rhs;
}
} // namespace s2h

#endif // S2H_MATH_MATRIX_H_