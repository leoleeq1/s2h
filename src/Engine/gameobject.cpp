#include "s2h/Engine/gameobject.h"
#include "s2h/Engine/Component/transform.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <queue>
#include <string>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace s2h
{
GameObject::GameObject()
{
  AddComponent<s2h::Transform>();
}

GameObject::GameObject(std::string name) : GameObject()
{
  name_ = name;
}

GameObject::~GameObject() = default;

std::string& GameObject::GetName() noexcept
{
  return name_;
}

Component *GameObject::GetComponent(std::type_index ti)
{
  auto it = components_.find(ti);
  if (it == components_.end())
  {
    return nullptr;
  }
  auto& [_, v] = *it;
  return v[0].get();
}

std::vector<Component *> GameObject::GetComponents(std::type_index ti)
{
  std::vector<Component *> components;
  auto it = components_.find(ti);
  if (it == components_.end())
  {
    return components;
  }

  auto& [_, v] = *it;
  for (auto& c : v)
  {
    components.push_back(c.get());
  }
  return components;
}

Component *GameObject::GetComponentInChildren(
  std::type_index ti, bool includeSelf)
{
  std::queue<Transform *> children;
  Component *component = nullptr;

  if (includeSelf)
  {
    children.push(&GetTransform());
  }
  else
  {
    auto& transform = GetTransform();
    std::size_t n = transform.GetChildrenSize();
    for (std::size_t i = 0; i < n; ++i)
    {
      children.push(transform.GetChild(i));
    }
  }

  while (!children.empty())
  {
    Transform *transform = children.front();
    children.pop();

    std::size_t n = transform->GetChildrenSize();
    for (std::size_t i = 0; i < n; ++i)
    {
      children.push(transform->GetChild(i));
    }

    component = transform->GetGameObject().GetComponent(ti);
    if (component)
    {
      return component;
    }
  }

  return nullptr;
}

std::vector<Component *> GameObject::GetComponentsInChildren(
  std::type_index ti, bool includeSelf)
{
  std::vector<Component *> components;
  std::queue<Transform *> children;

  if (includeSelf)
  {
    children.push(&GetTransform());
  }
  else
  {
    auto& transform = GetTransform();
    std::size_t n = transform.GetChildrenSize();
    for (std::size_t i = 0; i < n; ++i)
    {
      children.push(transform.GetChild(i));
    }
  }

  while (!children.empty())
  {
    Transform *transform = children.front();
    children.pop();

    std::size_t n = transform->GetChildrenSize();
    for (std::size_t i = 0; i < n; ++i)
    {
      children.push(transform->GetChild(i));
    }

    std::ranges::copy(transform->GetGameObject().GetComponents(ti),
      std::back_inserter(components));
  }

  return components;
}

s2h::Transform& GameObject::GetTransform() noexcept
{
  return *GetComponent<s2h::Transform>();
}
} // namespace s2h
