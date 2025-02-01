#ifndef S2H_ENGINE_OOP_GAMEOBJECT_H_
#define S2H_ENGINE_OOP_GAMEOBJECT_H_

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace s2h
{
class Component;
class Transform;

template<typename T>
concept ComponentType = std::is_base_of_v<Component, T>;

class GameObject
{
 public:
  GameObject();
  GameObject(std::string name);
  ~GameObject();

  std::string& GetName() noexcept;

  template<ComponentType T> T *GetComponent();
  template<ComponentType T> std::vector<T *> GetComponents();
  template<ComponentType T> T *GetComponentInChildren(bool includeSelf = false);
  template<ComponentType T>
  std::vector<T *> GetComponentsInChildren(bool includeSelf = false);

  Component *GetComponent(std::type_index ti);
  std::vector<Component *> GetComponents(std::type_index ti);
  Component *GetComponentInChildren(
    std::type_index ti, bool includeSelf = false);
  std::vector<Component *> GetComponentsInChildren(
    std::type_index ti, bool includeSelf = false);

  template<ComponentType T> T *AddComponent();

  Transform& GetTransform() noexcept;

 private:
  std::unordered_map<std::type_index, std::vector<std::unique_ptr<Component>>>
    components_;
  std::string name_;
};

template<ComponentType T> T *GameObject::GetComponent()
{
  std::type_index type = typeid(T);
  return reinterpret_cast<T *>(GetComponent(type));
}

template<ComponentType T> std::vector<T *> GameObject::GetComponents()
{
  std::type_index type = typeid(T);
  std::vector<T *> components;

  auto v = GetComponents(type);
  for (auto& c : v)
  {
    components.push_back(reinterpret_cast<T *>(c));
  }
  return components;
}

template<ComponentType T>
T *GameObject::GetComponentInChildren(bool includeSelf)
{
  std::type_index type = typeid(T);
  return reinterpret_cast<T *>(GetComponentInChildren(type, includeSelf));
}

template<ComponentType T>
std::vector<T *> GameObject::GetComponentsInChildren(bool includeSelf)
{
  std::type_index type = typeid(T);
  std::vector<T *> components;

  auto v = GetComponentsInChildren(type, includeSelf);
  for (auto& c : v)
  {
    components.push_back(reinterpret_cast<T *>(c));
  }
  return components;
}

template<ComponentType T> T *GameObject::AddComponent()
{
  std::type_index ti = std::type_index(typeid(T));
  auto& components = components_[ti];
  components.emplace_back(std::make_unique<T>());
  auto& ptr = components[components.size() - 1];
  ptr->gameObject_ = this;
  return reinterpret_cast<T *>(ptr.get());
}
} // namespace s2h

#endif // S2H_ENGINE_OOP_GAMEOBJECT_H_