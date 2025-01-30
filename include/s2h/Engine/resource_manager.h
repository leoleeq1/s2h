#ifndef S2H_ENGINE_RESOURCE_MANAGER_H_
#define S2H_ENGINE_RESOURCE_MANAGER_H_

#include "s2h/Resource/resource.h"

#include <concepts>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace s2h
{
template<typename T>
concept ResourceType = std::is_base_of_v<IResource, T>;

class ResourceManager
{
 public:
  template<ResourceType T> T *Add(std::unique_ptr<T> resource);

 private:
  std::unordered_map<std::type_index, std::vector<std::unique_ptr<IResource>>>
    resources_;
};

template<ResourceType T> T *ResourceManager::Add(std::unique_ptr<T> resource)
{
  auto& resources = resources_[std::type_index{typeid(T)}];
  resources.push_back(std::move(resource));
  return reinterpret_cast<T *>(resources[resources.size() - 1].get());
}
} // namespace s2h

#endif // S2H_ENGINE_RESOURCE_MANAGER_H_