#ifndef S2H_ENGINE_OOP_COMPONENT_COMPONENT_H_
#define S2H_ENGINE_OOP_COMPONENT_COMPONENT_H_

#include "s2h/Engine/OOP/gameobject.h"

#include <typeindex>
#include <typeinfo>
#include <vector>

namespace s2h
{
class Component
{
 public:
  virtual ~Component() = default;

  [[nodiscard]] GameObject& GetGameObject() noexcept;

  template<typename T> T *GetComponent();
  template<typename T> std::vector<T *> GetComponents();
  template<typename T> T *GetComponentInChildren(bool includeSelf = false);
  template<typename T>
  std::vector<T *> GetComponentsInChildren(bool includeSelf = false);

  Component *GetComponent(std::type_index ti);
  std::vector<Component *> GetComponents(std::type_index ti);
  Component *GetComponentInChildren(
    std::type_index ti, bool includeSelf = false);
  std::vector<Component *> GetComponentsInChildren(
    std::type_index ti, bool includeSelf = false);

  friend GameObject;

 protected:
  s2h::GameObject *gameObject_;
};

template<typename T> T *Component::GetComponent()
{
  return gameObject_->GetComponent<T>();
};

template<typename T> std::vector<T *> Component::GetComponents()
{
  return gameObject_->GetComponents<T>();
}

template<typename T> T *Component::GetComponentInChildren(bool includeSelf)
{
  return gameObject_->GetComponentInChildren<T>(includeSelf);
}

template<typename T>
std::vector<T *> Component::GetComponentsInChildren(bool includeSelf)
{
  return gameObject_->GetComponentsInChildren<T>(includeSelf);
}
} // namespace s2h

#endif // S2H_ENGINE_OOP_COMPONENT_COMPONENT_H_