#ifndef S2H_ENGINE_ECS_ECS_H_
#define S2H_ENGINE_ECS_ECS_H_

#include "s2h/Engine/ECS/System/system.h"
#include "s2h/Engine/ECS/entity.h"
#include "s2h/joined_span.h"

#include <algorithm>
#include <any>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <memory>
#include <ranges>
#include <span>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace s2h::ecs
{
using EntityId = int32_t;
using ComponentId = std::type_index;
using ArchetypeId = std::size_t;
using Row = std::size_t;
using Column = std::size_t;
using Type = std::vector<ComponentId>;

struct TypeHasher
{
  std::size_t operator()(const Type& components) const
  {
    std::vector<std::size_t> sorted{};
    std::ranges::transform(components, std::back_inserter(sorted),
      [](const ComponentId& id) { return id.hash_code(); });

    std::sort(sorted.begin(), sorted.end());

    ArchetypeId seed{};
    for (auto& comp : components)
    {
      seed ^=
        std::hash<ComponentId>{}(comp) + 0x9E3779B9 + (seed << 6) + (seed >> 2);
    }

    return seed;
  }
};

struct ComponentData
{
  template<typename T> T *GetElement(std::size_t i)
  {
    assert(i < size);
    return reinterpret_cast<T *>(elements.data() + elementSize * i);
  }
  void push_back(uint8_t *item);
  void erase(int64_t pos);

  std::vector<uint8_t> elements;
  std::size_t elementSize; // byte size of single element
  std::size_t size;        // number of elements
};

struct Archetype
{
  template<typename T> ComponentData& GetData() noexcept
  {
    return components[map[typeid(T)]];
  }

  template<std::ranges::range T> bool Contains(const T& types)
  {
    for (auto& t : types)
    {
      if (!map.contains(t))
      {
        return false;
      }
    }
    return true;
  }

  struct After
  {
    ArchetypeId add;
    ArchetypeId remove;
  };

  Type type;
  std::vector<Entity> entities;
  std::vector<ComponentData> components;
  std::unordered_map<ComponentId, Column> map;
  std::unordered_map<ComponentId, After> transition;
};

struct EntityRecord
{
  Archetype *archetype;
  Row row;
};

class ECS
{
 public:
  ~ECS() = default;

  ecs::Entity Entity();
  template<typename... Args> TypedSystem<Args...>& System();

  template<typename T, typename... Args>
  void AddComponent(ecs::Entity entity, Args... args);
  template<typename T> void RemoveComponent(ecs::Entity entity);
  template<typename T> T *GetComponent(ecs::Entity entity);

  void Update(float dt);
  template<typename... Args> void Execute(std::function<void(Args *...)> func);

  template<typename T, typename... Args>
  std::vector<std::tuple<T *, Args *...>> Query();

  void Reset();

 private:
  template<typename... Args>
  static ArchetypeId GetArchetypeId(
    std::span<ComponentId> components, Args... args);
  void MoveEntity(ecs::Entity record, Archetype& from, Archetype& to);

  std::unordered_map<ecs::Entity, EntityRecord> entityIndex_;
  std::unordered_map<ComponentId, std::unordered_set<Archetype *>>
    componentIndex_;
  std::unordered_map<ArchetypeId, Archetype> archetypeIndex_;
  std::unordered_map<ecs::Type, std::vector<std::unique_ptr<ecs::ISystem>>,
    ecs::TypeHasher>
    systemIndex_;
  EntityId nextEntityId_ = 0;
};

template<typename... Args> ecs::TypedSystem<Args...>& ECS::System()
{
  std::vector<ComponentId> components = {typeid(Args)...};
  std::sort(components.begin(), components.end());
  systemIndex_[components].push_back(
    std::make_unique<ecs::TypedSystem<Args...>>());
  return reinterpret_cast<ecs::TypedSystem<Args...>&>(
    *(*systemIndex_[components].rbegin()).get());
}

template<typename T, typename... Args>
void ECS::AddComponent(ecs::Entity entity, Args... args)
{
  ComponentId component = typeid(T);

  EntityRecord& record = entityIndex_[entity];
  Archetype *archetype = record.archetype;
  if (!archetype->transition[component].add)
  {
    archetype->transition[component].add =
      GetArchetypeId(archetype->type, component);
  }

  Archetype *to = &archetypeIndex_[archetype->transition[component].add];
  if (to->type.size() == 0)
  {
    std::ranges::copy(archetype->type, std::back_inserter(to->type));
    to->type.push_back(component);
    std::sort(to->type.begin(), to->type.end());
    for (auto& comp : to->type)
    {
      componentIndex_[comp].insert(to);
    }

    to->components.reserve(to->type.size());
    std::size_t col = 0;
    for (auto& cid : to->type)
    {
      to->map[cid] = col++;
      if (archetype->map.contains(cid))
      {
        to->components.push_back(ComponentData{
          .elementSize =
            archetype->components[archetype->map[cid]].elementSize});
      }
      else
      {
        to->components.push_back(ComponentData{.elementSize = sizeof(T)});
      }
    }
  }

  MoveEntity(entity, *archetype, *to);
  T element = T{args...};
  to->components[to->map[component]].push_back(
    reinterpret_cast<uint8_t *>(&element));
}

template<typename T> void ECS::RemoveComponent(ecs::Entity entity)
{
  ComponentId component = typeid(T);
  EntityRecord& record = entityIndex_[entity];
  Archetype *archetype = record.archetype;
  if (!archetype->transition[component].remove)
  {
    archetype->transition[component].remove =
      GetArchetypeId(archetype->type, component);
  }

  Archetype *to = &archetypeIndex_[archetype->transition[component].remove];
  if (to->type.size() == 0)
  {
    std::ranges::copy_if(archetype->type, std::back_inserter(to->type),
      [&component](ComponentId id) { return id != component; });
    std::sort(to->type.begin(), to->type.end());

    to->components.reserve(to->type.size());
    std::size_t col = 0;
    for (auto& cid : to->type)
    {
      to->map[cid] = col++;
    }
  }

  MoveEntity(entity, *archetype, *to);
}

template<typename T> T *ECS::GetComponent(ecs::Entity entity)
{
  ComponentId component = typeid(T);
  EntityRecord& record = entityIndex_[entity];
  Archetype& archetype = *record.archetype;

  if (!archetype.map.contains(component))
  {
    return nullptr;
  }

  return reinterpret_cast<void *>(
    archetype.components[archetype.map[component]].elements[record.row]);
}

template<typename... Args>
void ECS::Execute(std::function<void(Args *...)> func)
{
  std::array<ComponentId, sizeof...(Args)> components = {typeid(Args)...};
  std::ranges::filter_view types =
    componentIndex_[components[0]] | std::views::filter([&](auto *arche) {
      return arche->Contains(components);
    });

  for (s2h::ecs::Archetype *arche : types)
  {
    for (std::size_t i = 0; i < arche->entities.size(); ++i)
    {
      std::tuple<Args *...> params;

      ((std::get<Args *>(params) = reinterpret_cast<Args *>(
          arche->components[arche->map[typeid(Args)]].elements.data()
          + arche->components[arche->map[typeid(Args)]].elementSize * i)),
        ...);

      std::apply(func, params);
    }
  }
}

template<typename T, typename... Args>
std::vector<std::tuple<T *, Args *...>> ECS::Query()
{
  std::vector<std::tuple<T *, Args *...>> values;

  std::vector<ComponentId> components = {typeid(T), typeid(Args)...};
  std::ranges::filter_view types =
    componentIndex_[typeid(T)]
    | std::views::filter(
      [&](s2h::ecs::Archetype *arche) { return arche->Contains(components); });

  for (s2h::ecs::Archetype *arche : types)
  {
    for (std::size_t i = 0; i < arche->entities.size(); ++i)
    {
      std::tuple<T *, Args *...> params;

      std::get<T *>(params) = reinterpret_cast<T *>(
        arche->components[arche->map[typeid(T)]].elements.data()
        + arche->components[arche->map[typeid(T)]].elementSize * i);

      ((std::get<Args *>(params) = reinterpret_cast<Args *>(
          arche->components[arche->map[typeid(Args)]].elements.data()
          + arche->components[arche->map[typeid(Args)]].elementSize * i)),
        ...);

      values.push_back(params);
    }
  }

  return values;
}

template<typename... Args>
ArchetypeId ECS::GetArchetypeId(std::span<ComponentId> components, Args... args)
{
  const std::size_t cs = components.size();
  const std::size_t as = sizeof...(Args);
  const std::size_t total = cs + as;

  std::vector<ComponentId> sorted{};
  std::ranges::copy(components, std::back_inserter(sorted));
  ((sorted.push_back(static_cast<ComponentId>(args))), ...);

  std::sort(sorted.begin(), sorted.begin() + static_cast<ptrdiff_t>(total),
    std::less());
  ArchetypeId seed{};

  for (std::size_t i = 0; i < total; ++i)
  {
    seed ^= std::hash<ComponentId>{}(sorted.at(i)) + 0x9E3779B9 + (seed << 6)
            + (seed >> 2);
  }

  return seed;
}
} // namespace s2h::ecs

#endif // S2H_ENGINE_ECS_ECS_H_