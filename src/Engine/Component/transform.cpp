#include "s2h/Engine/Component/transform.h"
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
  return mat4{
    v4f::Basis(0),
    v4f::Basis(1),
    v4f::Basis(2),
    v4f{localPosition_[0], localPosition_[1], localPosition_[2], 1.0f},
  };
}

mat4 Transform::GetLocalRotationMatrix() const noexcept
{
  float cp = s2h::cos(s2h::numbers::fDeg2Rad * localRotation_[0]);
  float sp = s2h::sin(s2h::numbers::fDeg2Rad * localRotation_[0]);
  float cy = s2h::cos(s2h::numbers::fDeg2Rad * localRotation_[1]);
  float sy = s2h::sin(s2h::numbers::fDeg2Rad * localRotation_[1]);
  float cr = s2h::cos(s2h::numbers::fDeg2Rad * localRotation_[2]);
  float sr = s2h::sin(s2h::numbers::fDeg2Rad * localRotation_[2]);

  return mat4{
    v4f{cr * cy + sr * sp * sy,  -sr * cy + cr * sp * sy, cp * sy, 0.0f},
    v4f{sr * cp,                 cr * cp,                 -sp,     0.0f},
    v4f{-sy * cr + sr * sp * cy, sr * sy + cr * sp * cy,  cp * cy, 0.0f},
    v4f::Basis(3),
  };
}

mat4 Transform::GetLocalScaleMatrix() const noexcept
{
  return mat4{
    v4f::Basis(0) * localScale_[0],
    v4f::Basis(1) * localScale_[1],
    v4f::Basis(2) * localScale_[2],
    v4f::Basis(3),
  };
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

mat4 Transform::GetViewMatrix() const noexcept
{
  mat4 t = mat4{
    v4f::Basis(0),
    v4f::Basis(1),
    v4f::Basis(2),
    v4f{-position_, 1.0f},
  };

  mat4 r = s2h::Transpose(GetLocalRotationMatrix());
  //   mat4 r = s2h::Transpose(mat4{
  //   v4f{right_,   0.0f},
  //   v4f{up_,      0.0f},
  //   v4f{forward_, 0.0f},
  //   v4f::Basis(3)
  // });

  mat4 vm = t * r;
  return parent_ == nullptr ? vm : vm * parent_->GetViewMatrix();
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