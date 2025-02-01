#ifndef S2H_ENGINE_ECS_SYSTEM_SYSTEM_H_
#define S2H_ENGINE_ECS_SYSTEM_SYSTEM_H_

#include <any>
#include <functional>
#include <tuple>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace s2h::ecs
{
class ISystem
{
 public:
  virtual ~ISystem() = default;
  virtual void Execute(
    std::unordered_map<std::type_index, void *>& components) = 0;
};

template<typename... Components> class TypedSystem : public ISystem
{
  using Func = std::function<void(Components&...)>;

 public:
  virtual ~TypedSystem() = default;

  void Each(Func func) { func_ = func; }

  void Execute(std::unordered_map<std::type_index, void *>& components) override
  {
    auto tuple = GetComponentsTuple(components);
    std::apply(func_, tuple);
  }

 private:
  std::tuple<Components...> GetComponentsTuple(
    std::unordered_map<std::type_index, void *>& components)
  {
    return std::tuple<Components...>(
      *reinterpret_cast<Components *>(components[typeid(Components)])...);
  }

  Func func_;
};
} // namespace s2h::ecs

#endif // S2H_ENGINE_ECS_SYSTEM_SYSTEM_H_