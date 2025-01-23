#ifndef S2H_MATRIX_H_
#define S2H_MATRIX_H_

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

template<std::size_t Row, std::size_t Col> struct mat
{
  float m[Row][Col];
};

#endif // S2H_MATRIX_H_