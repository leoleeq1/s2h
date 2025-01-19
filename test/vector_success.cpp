#include "s2h/vector.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <vector>

using namespace s2h;
using TestFn = int (*)();

int vector_add()
{
  v4f v1{1.0f, -1.0f, 1000.0f, -20000.0f};
  v4f v2(-v1);

  auto add = v1 + v2;
  for (auto v : add.v)
  {
    if (v != 0.0f)
    {
      printf("vector_add Failed!\n");
      return 0;
    }
  }

  return 1;
}

int vector_subtract()
{
  v4f v1{1.0f, -1.0f, 1000.0f, -20000.0f};
  v4f v2(v1);

  auto subtract = v1 - v2;
  for (auto v : subtract.v)
  {
    if (v != 0.0f)
    {
      printf("vector_subtract Failed!\n");
      return 0;
    }
  }

  return 1;
}

int vector_multiply()
{
  v4f v1{1.0f, -1.0f, 1000.0f, -20000.0f};

  auto multiply = v1 * 0.0f;
  for (auto v : multiply.v)
  {
    if (v != 0.0f)
    {
      printf("vector_multiply Failed!\n");
      return 0;
    }
  }

  return 1;
}

int vector_divide()
{
  v4f v1{1.0f, -1.0f, 1000.0f, -20000.0f};

  auto divide = v1 / 1.0f;
  for (std::size_t i = 0; i < 4; ++i)
  {
    if (divide[i] != v1[i])
    {
      printf("vector_divide Failed!\n");
      return 0;
    }
  }

  return 1;
}

int vector_dot()
{
  v4f v1{1.0f, -1.0f, 1000.0f, -20000.0f};
  v4f v2{1.0f, 1.0f, 1.0f, 1.0f};

  auto result = dot(v1, v2);

  float sum = 0.0f;
  for (auto v : v1.v)
  {
    sum += v;
  }

  if (result != sum)
  {
    printf("vector_dot Failed!\n");
    return 0;
  }

  return 1;
}

int vector_cross()
{
  v3f v1{1.0f, 0.0f, 0.0f};
  v3f v2{0.0f, 1.0f, 0.0f};
  v3f v3{0.0f, 0.0f, 1.0f};

  auto v4 = cross(v1, v2);
  if (!equals(v3, v4))
  {
    printf("vector_cross Failed!\n");
    return 0;
  }

  return 1;
}

int vector_success(int, char *[])
{
  std::vector<TestFn> tests{
    vector_add,
    vector_subtract,
    vector_multiply,
    vector_divide,
    vector_dot,
    vector_cross,
  };

  std::size_t success = 0;
  for (auto& test : tests)
  {
    success += test();
  }

  return (success == tests.size()) ? 0 : -1;
}