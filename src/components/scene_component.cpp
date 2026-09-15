#include "scene_component.h"

#include "../core/actor.h"


namespace hge
{
  HGE_SceneComponent::HGE_SceneComponent(HGE_Actor* _parent) : HGE_Component(_parent)
  {
    HPROPERTY(relative_transform, Exposed, TransformModified());
  }

  HGE_Transform HGE_SceneComponent::GetAbsoluteTransform()
  {
    return relative_transform * parent_->transform;
  }

  HGE_Vec3 HGE_SceneComponent::GetAbsoluteLocation()
  {
    return relative_transform.location_ + parent_->transform.location_;
  }
  HGE_Vec3 HGE_SceneComponent::GetAbsoluteRotation()
  {
    return relative_transform.rotation_ + parent_->transform.rotation_;
  }
  HGE_Vec3 HGE_SceneComponent::GetAbsoluteScale()
  {
    return relative_transform.scale_ * parent_->transform.scale_;
  }
}