#include "s2h/Engine/ECS/ecs.h"
#include "s2h/Engine/ECS/System/system.h"
#include "s2h/Engine/ECS/entity.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <queue>
#include <span>
#include <unordered_map>
#include <vector>

namespace s2h::ecs
{
void s2h::ecs::ComponentData::push_back(uint8_t *item)
{
  if (elementSize * size < elements.size())
  {
    for (std::size_t i = 0; i < elementSize; ++i)
    {
      elements[elementSize * size + i] = (*(item + i));
    }
  }
  else
  {
    for (std::size_t i = 0; i < elementSize; ++i)
    {
      elements.push_back(*(item + i));
    }
  }
  ++size;
}

void s2h::ecs::ComponentData::erase(int64_t pos)
{
  assert(size > 0);
  int64_t last = static_cast<int64_t>(elementSize * (size - 1));
  std::copy(elements.begin() + last,
    elements.begin() + last + static_cast<int64_t>(elementSize),
    elements.begin() + pos);
  --size;
}

ecs::Entity ECS::Entity()
{
  ecs::Entity e{nextEntityId_++};
  archetypeIndex_[0].entities.push_back(e);
  entityIndex_[e] =
    ecs::EntityRecord{&archetypeIndex_[0], static_cast<ecs::Row>(-1)};
  return e;
}

void ECS::Update(float dt)
{
  for (auto& [comps, systems] : systemIndex_)
  {
    auto types =
      componentIndex_[comps[0]]
      | std::views::filter([&](auto *arche) { return arche->Contains(comps); });

    for (auto& system : systems)
    {
      for (auto& arche : types)
      {
        for (auto& e : arche->entities)
        {
          std::unordered_map<std::type_index, void *> components;
          ecs::EntityRecord& record = entityIndex_[e];
          for (auto& t : comps)
          {
            components[t] =
              arche->components[arche->map[t]].elements.data()
              + arche->components[arche->map[t]].elementSize * record.row;
          }
          system->Execute(dt, components);
        }
      }
    }
  }
}

void ECS::Reset()
{
  entityIndex_.clear();
  componentIndex_.clear();
  archetypeIndex_.clear();
  nextEntityId_ = 0;
}

void ECS::MoveEntity(
  ecs::Entity entity, ecs::Archetype& from, ecs::Archetype& to)
{
  ecs::EntityRecord& record = entityIndex_[entity];

  to.entities.push_back(entity);
  ecs::Row newRow = to.entities.size() - 1;
  for (auto& [cid, col] : from.map)
  {
    if (!to.map.contains(cid)) continue;

    uint8_t *item = &from.components[col].elements[record.row];
    auto& toComp = to.components[to.map[cid]];

    if (to.entities.capacity()
        != (toComp.elements.capacity() / toComp.elementSize))
    {
      toComp.elements.reserve(to.entities.capacity() * toComp.elementSize);
    }
    toComp.push_back(item);
  }

  for (auto& component : from.components)
  {
    component.erase(static_cast<int64_t>(component.elementSize * record.row));
  }
  from.entities.erase(
    std::remove(from.entities.begin(), from.entities.end(), entity),
    from.entities.end());

  record.archetype = &to;
  record.row = newRow;
}
} // namespace s2h::ecs