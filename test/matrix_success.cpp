#include "s2h/Math/matrix.h"
#include "s2h/Math/vector.h"

#include <span>
#include <vector>

using namespace s2h;
using TestFn = int (*)();

int matrix_constructor()
{
  std::array<float, 16> arr{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  std::array<vec<float, 4>, 4> arr2{
    v4f{0,  1,  2,  3 },
    v4f{4,  5,  6,  7 },
    v4f{8,  9,  10, 11},
    v4f{12, 13, 14, 15}
  };

  mat<float, 4> m0{arr};
  mat<float, 4> m1{arr2};
  mat<float, 4> m2{m1};
  mat<float, 4> m3{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  return 1;
}

int matrix_success(int, char *[])
{
  std::vector<TestFn> tests{
    matrix_constructor,
  };

  std::size_t success = 0;
  for (auto& test : tests)
  {
    success += test();
  }

  return (success == tests.size()) ? 0 : -1;
}