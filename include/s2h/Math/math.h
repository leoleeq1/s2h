#ifndef S2H_MATH_MATH_H_
#define S2H_MATH_MATH_H_

#include <bit>
#include <cassert>
#include <cmath>
#include <concepts>
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
constexpr float fHalf_Pi = std::numbers::pi_v<float> * 0.5f;
constexpr float fInv_Pi = std::numbers::inv_pi_v<float>;
constexpr float fInv_SqrtPi = std::numbers::inv_sqrtpi_v<float>;
constexpr float fRad2Deg = 57.295779513082320876798154814105f;
constexpr float fDeg2Rad = 0.01745329251994329576923690768489f;

constexpr double E = std::numbers::e_v<double>;
constexpr double Pi = std::numbers::pi_v<double>;
constexpr double Half_Pi = std::numbers::pi_v<double> * 0.5;
constexpr double Inv_Pi = std::numbers::inv_pi_v<double>;
constexpr double Inv_SqrtPi = std::numbers::inv_sqrtpi_v<double>;
constexpr double Rad2Deg = 57.295779513082320876798154814105;
constexpr double Deg2Rad = 0.01745329251994329576923690768489f;
} // namespace numbers

template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template<typename T>
concept HasArithmeticOp = requires(T a, T b) {
  a + b;
  a - b;
  a *b;
  a / b;
};

namespace
{
template<typename T>
  requires std::is_arithmetic_v<T>
struct same_size_integral
{
  using type = std::conditional_t<sizeof(T) == sizeof(int8_t), int8_t,
    std::conditional_t<sizeof(T) == sizeof(int16_t), int16_t,
      std::conditional_t<sizeof(T) == sizeof(int32_t), int32_t,
        std::conditional_t<sizeof(T) == sizeof(int64_t), int64_t, void>>>>;
};

template<typename T>
  requires std::is_arithmetic_v<T>
struct same_size_float
{
  using type = std::conditional_t<sizeof(T) <= sizeof(float), float,
    std::conditional_t<sizeof(T) == sizeof(double), double,
      std::conditional_t<sizeof(T) == sizeof(long double), long double, void>>>;
};

template<typename T>
using same_size_float_t = typename same_size_float<T>::type;

template<typename T> consteval size_t BitCount() noexcept
{
  return 8 * sizeof(T);
}
} // namespace

constexpr bool isinf(auto num) noexcept
{
  return std::isinf(num);
}

constexpr bool isnan(auto num) noexcept
{
  return std::isnan(num);
}

constexpr bool isnegative(auto num) noexcept
{
  return std::signbit(num);
}

constexpr auto signbit(auto num) noexcept
{
  return std::signbit(num) ? static_cast<decltype(num)>(-1)
                           : static_cast<decltype(num)>(1);
}

constexpr auto min(auto a, auto b) noexcept
{
  return a > b ? b : a;
}

constexpr auto max(auto a, auto b) noexcept
{
  return a > b ? a : b;
}

constexpr auto abs(auto num) noexcept
{
  return std::abs(num);
}

// e^num
constexpr auto exp(auto num) noexcept
{
  return std::exp(num);
}

// 2^num
constexpr auto exp2(auto num) noexcept
{
  return std::exp2(num);
}

// base^exp
constexpr auto pow(auto base, auto exp) noexcept
{
  return std::pow(base, exp);
}

constexpr auto sqrt(auto num) noexcept
{
  assert(num >= 0 && !s2h::isnan(num));
  return std::sqrt(num);
}

constexpr float rsqrt(float num) noexcept
{
  const auto y =
    std::bit_cast<float>(0x5F3759DF - (std::bit_cast<uint32_t>(num) >> 1));
  return y * (1.5f - (num * 0.5f * y * y));
}

constexpr auto ceil(auto num) noexcept
{
  return std::ceil(num);
}

constexpr auto floor(auto num) noexcept
{
  return std::floor(num);
}

constexpr auto trunc(auto num) noexcept
{
  return std::trunc(num);
}

// a + t(b - a)
constexpr auto lerp(
  Arithmetic auto a, Arithmetic auto b, Arithmetic auto t) noexcept
{
  return std::lerp(a, b, t);
}

// a + t(b - a) for non arithmetic types
constexpr HasArithmeticOp auto lerp(HasArithmeticOp auto a,
  HasArithmeticOp auto b, HasArithmeticOp auto t) noexcept
{
  return a + t * (b - a);
}

constexpr auto sin(auto rad) noexcept
{
  assert(!s2h::isinf(rad) && !s2h::isnan(rad));
  return std::sin(rad);
}

constexpr auto cos(auto rad) noexcept
{
  assert(!s2h::isinf(rad) && !s2h::isnan(rad));
  return std::cos(rad);
}

constexpr auto tan(auto rad) noexcept
{
  assert(rad != 0 && !s2h::isinf(rad) && !s2h::isnan(rad));
  return std::tan(rad);
}

constexpr auto asin(auto num) noexcept
{
  assert(abs(num) <= 1 && !s2h::isnan(num));
  return std::asin(num);
}

constexpr auto acos(auto num) noexcept
{
  assert(abs(num) <= 1 && !s2h::isnan(num));
  return std::acos(num);
}

constexpr auto atan(auto num) noexcept
{
  assert(!s2h::isnan(num));
  return std::atan(num);
}

constexpr auto atan2(auto y, auto x) noexcept
{
  assert(!s2h::isnan(y) && !s2h::isnan(x));
  return std::atan2(y, x);
}

template<typename T>
  requires std::is_floating_point_v<T>
constexpr bool isNearlyZero(T value)
{
  return std::abs(value) <= std::numeric_limits<T>::epsilon();
}

template<typename T>
  requires std::is_floating_point_v<T>
constexpr bool equals(T lhs, T rhs,
  T epsilon = std::numeric_limits<T>::epsilon(),
  int32_t maxUlpDiff = 4ULL) noexcept
{
  assert(maxUlpDiff > 0 && maxUlpDiff <= 4);

  float absDiff = abs(lhs - rhs);
  if (absDiff <= epsilon)
  {
    return true;
  }

  if (s2h::isnegative(lhs) != s2h::isnegative(rhs))
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
} // namespace s2h

#endif // S2H_MATH_MATH_H_