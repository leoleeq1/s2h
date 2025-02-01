#ifndef S2H_ENGINE_OOP_SCENE_H_
#define S2H_ENGINE_OOP_SCENE_H_

#include <memory>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

namespace s2h
{
class GameObject;
class Scene
{
 public:
  ~Scene();

  s2h::GameObject *CreateGameObject();
  s2h::GameObject *CreateGameObject(std::string name);

  s2h::GameObject *FindGameObjectWithName(std::string name);
  template<typename T> std::vector<T *> FindComponents();

 private:
  std::vector<std::unique_ptr<s2h::GameObject>> gameObjects_;
};

template<typename T> std::vector<T *> Scene::FindComponents()
{
  std::vector<T *> components;
  for (auto& go : gameObjects_)
  {
    T *component = go->template GetComponent<T>();
    if (component)
    {
      components.push_back(component);
    }
  }

  return components;
}
} // namespace s2h

#endif // S2H_ENGINE_OOP_SCENE_H_