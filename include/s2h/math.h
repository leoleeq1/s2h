#ifndef S2H_MATH_H_
#define S2H_MATH_H_

#include <cassert>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numbers>
#include <type_traits>

namespace s2h
{
namespace numbers
{
constexpr float fE = std::numbers::e_v<float>;
constexpr float fPi = std::numbers::pi_v<float>;
constexpr float fInv_Pi = std::numbers::inv_pi_v<float>;
constexpr float fInv_SqrtPi = std::numbers::inv_sqrtpi_v<float>;
constexpr float fRad2Deg = 57.295779513082320876798154814105f;
constexpr float fDeg2Rad = 0.01745329251994329576923690768489f;

constexpr double E = std::numbers::e_v<double>;
constexpr double Pi = std::numbers::pi_v<double>;
constexpr double Inv_Pi = std::numbers::inv_pi_v<double>;
constexpr double Inv_SqrtPi = std::numbers::inv_sqrtpi_v<double>;
constexpr double Rad2Deg = 57.295779513082320876798154814105;
constexpr double Deg2Rad = 0.01745329251994329576923690768489f;
} // namespace numbers

namespace
{
template<typename T> struct same_size_integral;
template<> struct same_size_integral<float>
{
  using type =
    std::conditional_t<sizeof(float) == sizeof(int32_t), int32_t, void>;
};

template<> struct same_size_integral<double>
{
  using type =
    std::conditional_t<sizeof(double) == sizeof(int64_t), int64_t, void>;
};

template<> struct same_size_integral<long double>
{
  using type =
    std::conditional_t<sizeof(long double) == sizeof(int64_t), int64_t, void>;
};

template<typename T> consteval size_t BitCount()
{
  return 8 * sizeof(T);
}
} // namespace

template<typename T>
  requires std::is_floating_point_v<T>
bool equals(T lhs, T rhs, T epsilon = std::numeric_limits<T>::epsilon(),
  int32_t maxUlpDiff = 4ULL)
{
  assert(maxUlpDiff > 0 && maxUlpDiff <= 4);

  float absDiff = abs(lhs - rhs);
  if (absDiff <= epsilon)
  {
    return true;
  }

  if (std::signbit(lhs) != std::signbit(rhs))
  {
    return false;
  }

  int32_t ulpDiff =
    abs(*reinterpret_cast<same_size_integral<T>::type *>(&lhs)
        - *reinterpret_cast<same_size_integral<T>::type *>(&rhs));
  if (ulpDiff <= maxUlpDiff)
  {
    return true;
  }

  return false;
}

auto min(auto a, auto b)
{
  return a > b ? b : a;
}

auto max(auto a, auto b)
{
  return a > b ? a : b;
}
} // namespace s2h

#endif // S2H_MATH_H_