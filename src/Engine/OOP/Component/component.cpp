#include "s2h/Engine/OOP/Component/component.h"
#include "s2h/Engine/OOP/gameobject.h"

namespace s2h
{
GameObject& Component::GetGameObject() noexcept
{
  return *gameObject_;
}

Component *Component::GetComponent(std::type_index ti)
{
  return gameObject_->GetComponent(ti);
}

std::vector<Component *> Component::GetComponents(std::type_index ti)
{
  return gameObject_->GetComponents(ti);
}

Component *Component::GetComponentInChildren(
  std::type_index ti, bool includeSelf)
{
  return gameObject_->GetComponentInChildren(ti, includeSelf);
}

std::vector<Component *> Component::GetComponentsInChildren(
  std::type_index ti, bool includeSelf)
{
  return gameObject_->GetComponentsInChildren(ti, includeSelf);
}
} // namespace s2h
