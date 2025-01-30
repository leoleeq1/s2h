#ifndef S2H_PROPERTY_H_
#define S2H_PROPERTY_H_

#include <functional>

namespace s2h
{
template<typename T> class Property
{
  using Getter = std::function<void()>;
  using Setter = std::function<void(T)>;

 public:
  Property(T& value) : value_{value} {}
  Property(T& value, Setter setter) : value_{value}, setter_{setter} {}
  Property(T& value, Getter getter) : value_{value}, getter_{getter} {}
  Property(T& value, Getter getter, Setter setter)
    : value_{value},
      getter_{getter},
      setter_{setter}
  {
  }
  Property(const Property&) = delete;
  Property& operator=(const Property&) = delete;

  void SetGetter(Getter getter) { getter_ = getter; }
  void SetSetter(Setter setter) { setter_ = setter; }
  void SetProperty(Getter getter, Setter setter)
  {
    getter_ = getter;
    setter_ = setter;
  }

  Property& operator+=(T value)
  {
    value_ += value;
    if (setter_)
    {
      setter_(value);
    }
    return *this;
  }

  Property& operator-=(T value)
  {
    value_ -= value;
    if (setter_)
    {
      setter_(value);
    }
    return *this;
  }

  Property& operator*=(T value)
  {
    value_ *= value;
    if (setter_)
    {
      setter_(value);
    }
    return *this;
  }

  Property& operator/=(T value)
  {
    value_ /= value;
    if (setter_)
    {
      setter_(value);
    }
    return *this;
  }

  Property& operator%=(T value)
  {
    value_ %= value;
    if (setter_)
    {
      setter_(value);
    }
    return *this;
  }

  Property& operator=(T value)
  {
    value_ = value;
    if (setter_)
    {
      setter_(value);
    }
    return *this;
  }

  operator T() const
  {
    if (getter_)
    {
      getter_();
    }
    return value_;
  }

 private:
  T& value_{};
  Getter getter_{};
  Setter setter_{};
};
} // namespace s2h

#endif // S2H_PROPERTY_H_