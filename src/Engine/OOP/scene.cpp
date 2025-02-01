#include "s2h/Engine/OOP/scene.h"
#include "s2h/Engine/OOP/gameobject.h"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace s2h
{
s2h::Scene::~Scene() = default;

s2h::GameObject *s2h::Scene::CreateGameObject()
{
  gameObjects_.push_back(std::make_unique<s2h::GameObject>());
  return gameObjects_[gameObjects_.size() - 1].get();
}

s2h::GameObject *s2h::Scene::CreateGameObject(std::string name)
{
  gameObjects_.push_back(std::make_unique<s2h::GameObject>(name));
  return gameObjects_[gameObjects_.size() - 1].get();
}

s2h::GameObject *s2h::Scene::FindGameObjectWithName(std::string name)
{
  for (std::size_t i = 0; i < gameObjects_.size(); ++i)
  {
    if (gameObjects_[i]->GetName() == name)
    {
      return gameObjects_[i].get();
    }
  }

  return nullptr;
}
} // namespace s2h
