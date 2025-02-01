#include "s2h/Engine/OOP/Component/transform.h"
#include "s2h/Math/math.h"
#include "s2h/Math/matrix.h"
#include "s2h/Math/vector.h"

namespace s2h
{
Transform::Transform()
{
  Update();
}

Transform::Transform(s2h::v3f position, s2h::v3f rotation, s2h::v3f scale)
  : localPosition_{position},
    localRotation_{rotation},
    localScale_{scale}
{
  Update();
}

void Transform::SetLocalPositionRotation(
  s2h::v3f position, s2h::v3f rotation) noexcept
{
  localPosition_ = position;
  localRotation_ = rotation;
  Update();
}

void Transform::SetPositionRotation(
  s2h::v3f position, s2h::v3f rotation) noexcept
{
  position_ = position;
  rotation_ = rotation;

  localPosition_ = parent_ == nullptr
                     ? position
                     : v3f(v4f(position) * parent_->GetInvWorldMatrix());
  localRotation_ =
    parent_ == nullptr ? rotation : rotation - parent_->rotation_;
  Update();
}

mat4 Transform::GetLocalTranslateMatrix() const noexcept
{
  return GetTranslateMatrix(localPosition_);
}

mat4 Transform::GetLocalRotationMatrix() const noexcept
{
  return GetRotationMatrix(localRotation_);
}

mat4 Transform::GetLocalScaleMatrix() const noexcept
{
  return GetScaleMatrix(localScale_);
}

mat4 Transform::GetModelMatrix() const noexcept
{
  mat4 t = GetLocalTranslateMatrix();
  mat4 r = GetLocalRotationMatrix();
  mat4 s = GetLocalScaleMatrix();
  return s * r * t;
}

mat4 Transform::GetInvModelMatrix() const noexcept
{
  mat4 t = mat4{
    v4f::Basis(0),
    v4f::Basis(1),
    v4f::Basis(2),
    v4f{-localPosition_, 1.0f},
  };
  mat4 r = s2h::Transpose(GetLocalRotationMatrix());
  mat4 s = 1.0f / GetLocalScaleMatrix();
  return s * r * t;
}

mat4 Transform::GetWorldMatrix() const noexcept
{
  return worldTrs_;
}

mat4 Transform::GetInvWorldMatrix() const noexcept
{
  return parent_ == nullptr
           ? GetInvModelMatrix()
           : GetInvModelMatrix() * parent_->GetInvWorldMatrix();
}

Transform *Transform::GetParent() noexcept
{
  return parent_;
}

Transform *Transform::GetChild(std::size_t i) noexcept
{
  assert(i >= children_.size());
  return children_[i];
}

std::size_t Transform::GetChildrenSize() const noexcept
{
  return children_.size();
}

void Transform::SetParent(Transform *parent) noexcept
{
  for (Transform *cur = parent; cur != nullptr; cur = cur->parent_)
  {
    if (cur == this)
    {
      return;
    }
  }

  if (parent_ != nullptr)
  {
    auto it = std::ranges::find(parent_->children_, this);
    assert(it == parent_->children_.end());
    parent_->children_.erase(it);
    parent_ = nullptr;
  }

  if (parent == nullptr)
  {
    localPosition_ = position_;
    localRotation_ = rotation_;
    localScale_ = scale_;
    Update();
    return;
  }

  parent_ = parent;
  parent_->children_.push_back(this);

  localPosition_ = v4f(localPosition_, 1.0f) * parent_->GetInvWorldMatrix();
  localRotation_ = localRotation_ - parent_->rotation_;
  localScale_ = localScale_ / parent_->scale_;

  Update();
}

void Transform::Update() noexcept
{
  mat4 trs = GetModelMatrix();

  if (parent_ == nullptr)
  {
    position_ = localPosition_;
    rotation_ = localRotation_;
    scale_ = localScale_;
    worldTrs_ = trs;
  }
  else
  {
    position_ = v4f(localPosition_) * parent_->worldTrs_;
    rotation_ = localRotation_ + parent_->localRotation_;
    scale_ = localScale_ * scale_;
    worldTrs_ = trs * parent_->worldTrs_;
  }

  for (Transform *child : children_)
  {
    child->Update();
  }
}
} // namespace s2h