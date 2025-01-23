#ifndef S2H_APPLICATION_H_
#define S2H_APPLICATION_H_

namespace s2h
{
class Application
{
 public:
  Application() = default;
  ~Application() = default;
  Application(const Application&) = delete;
  Application& operator=(const Application&) = delete;

  static void Update(float dt, Application *app);
  static void FixedUpdate(float dt, Application *app);

 private:
};
} // namespace s2h

#endif // S2H_APPLICATION_H_