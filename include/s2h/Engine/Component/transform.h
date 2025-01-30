#ifndef S2H_ENGINE_TRANSFORM_H_
#define S2H_ENGINE_TRANSFORM_H_

#include "s2h/Engine/Component/component.h"
#include "s2h/Math/matrix.h"
#include "s2h/Math/vector.h"
#include "s2h/property.h"

#include <cassert>
#include <vector>

namespace s2h
{
class Transform : public Component
{
 public:
  Transform();
  Transform(s2h::v3f position, s2h::v3f rotation, s2h::v3f scale);

  void SetLocalPositionRotation(s2h::v3f position, s2h::v3f rotation) noexcept;
  void SetPositionRotation(s2h::v3f position, s2h::v3f rotation) noexcept;

  [[nodiscard]] mat4 GetLocalTranslateMatrix() const noexcept;
  [[nodiscard]] mat4 GetLocalRotationMatrix() const noexcept;
  [[nodiscard]] mat4 GetLocalScaleMatrix() const noexcept;
  [[nodiscard]] mat4 GetModelMatrix() const noexcept;
  [[nodiscard]] mat4 GetInvModelMatrix() const noexcept;

  [[nodiscard]] mat4 GetWorldMatrix() const noexcept;
  [[nodiscard]] mat4 GetInvWorldMatrix() const noexcept;

  [[nodiscard]] mat4 GetViewMatrix() const noexcept;

  [[nodiscard]] Transform *GetParent() noexcept;
  [[nodiscard]] Transform *GetChild(std::size_t i) noexcept;
  [[nodiscard]] std::size_t GetChildrenSize() const noexcept;

  void SetParent(Transform *parent) noexcept;

  void Update() noexcept;

  s2h::Property<s2h::v3f> LocalPosition{
    localPosition_, [this](s2h::v3f) { this->Update(); }};

  s2h::Property<s2h::v3f> LocalRotation{
    localRotation_, [this](s2h::v3f) { this->Update(); }};

  s2h::Property<s2h::v3f> LocalScale{
    localScale_, [this](s2h::v3f) { this->Update(); }};

  s2h::Property<s2h::v3f> Position{
    position_, [this](s2h::v3f) {
      this->localPosition_ = this->parent_ == nullptr
                               ? this->position_
                               : v3f(v4f(this->position_, 1.0f)
                                     * this->parent_->GetInvWorldMatrix());
      this->Update();
    }};

  s2h::Property<s2h::v3f> Rotation{
    rotation_, [this](s2h::v3f) {
      this->localRotation_ = this->parent_ == nullptr
                               ? this->rotation_
                               : this->rotation_ - this->parent_->rotation_;
      this->Update();
    }};

  s2h::Property<s2h::v3f> Scale{scale_, [this](s2h::v3f) {
                                  this->localScale_ =
                                    this->parent_ == nullptr
                                      ? this->scale_
                                      : this->scale_ / this->parent_->scale_;
                                  this->Update();
                                }};

 private:
  s2h::mat4 worldTrs_{};

  s2h::v3f localPosition_{0.0f, 0.0f, 0.0f};
  s2h::v3f localRotation_{0.0f, 0.0f, 0.0f};
  s2h::v3f localScale_{1.0f, 1.0f, 1.0f};

  s2h::v3f position_{0.0f, 0.0f, 0.0f};
  s2h::v3f rotation_{0.0f, 0.0f, 0.0f};
  s2h::v3f scale_{1.0f, 1.0f, 1.0f};

  s2h::v3f right_{1.0f, 0.0f, 0.0f};
  s2h::v3f up_{0.0f, 1.0f, 0.0f};
  s2h::v3f forward_{0.0f, 0.0f, 1.0f};

  std::vector<Transform *> children_;
  Transform *parent_ = nullptr;
};
} // namespace s2h

#endif // S2H_ENGINE_TRANSFORM_H_