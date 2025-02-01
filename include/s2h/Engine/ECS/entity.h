#ifndef S2H_ENGINE_ECS_ENTITY_H_
#define S2H_ENGINE_ECS_ENTITY_H_

#include <cstdint>
#include <unordered_map>

namespace s2h::ecs
{
struct Entity
{
  int32_t id;
  bool operator==(const Entity&) const = default;
};
} // namespace s2h::ecs

template<> struct std::hash<s2h::ecs::Entity>
{
  std::size_t operator()(const s2h::ecs::Entity& e) const noexcept
  {
    return std::hash<int32_t>{}(e.id);
  }
};

#endif // S2H_ENGINE_ECS_ENTITY_H_