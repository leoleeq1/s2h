#ifndef S2H_JOINED_SPAN_H_
#define S2H_JOINED_SPAN_H_

#include <cassert>
#include <span>
#include <vector>

namespace s2h
{
template<typename T> class JoinedSpan
{
 public:
  void Join(std::span<T> span)
  {
    spans_.push_back(span);
    size_ += span.size();
  }

  std::size_t size() { return size_; }

  T& operator[](std::size_t index)
  {
    assert(index < size_);

    for (auto span : spans_)
    {
      if (index < span.size())
      {
        return span[index];
      }
      index -= span.size();
    }
  }

 private:
  std::vector<std::span<T>> spans_;
  std::size_t size_{};
};
} // namespace s2h

#endif // S2H_JOINED_SPAN_H_