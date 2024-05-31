#pragma once

#include "collision_type.h"
#include "collision_object.h"

class geometry_containment
{

public:

  geometry_containment(collision_type collisionType) : m_type { collisionType }
  {
  }

  auto operator()(collision_object& containedObject, collision_object& containmentObject, auto&& callable) -> void;
  auto operator()(collision_object& containedObject, collision_object& containmentObject) -> bool;

private:

  static [[nodiscard]] auto CheckDirect2D(collision_object &containedObject, collision_object &containmentObject) noexcept -> bool;

private:

  collision_type m_type;

};

auto geometry_containment::operator()(collision_object &containedObject, collision_object &containmentObject, auto &&callable) -> void
{
  auto containedPosition = containedObject.Object().Position();
  auto containmentPosition = containmentObject.Object().Position();

  auto containedBounds = containedObject.Bounds();
  auto containerBounds = containmentObject.Bounds();

  auto containedX = containerBounds.right > containedBounds.right && containerBounds.left < containedBounds.left;
  auto containedY = containerBounds.top < containedBounds.top && containerBounds.bottom > containedBounds.bottom;
  auto possibleCollision = containedX && containedY;

  switch( m_type )
  {
    case collision_type::direct2d:
      if( possibleCollision && CheckDirect2D(containedObject, containmentObject) )
      {
        callable(containedObject.Object(), containmentObject.Object());
      }
      break;

    case collision_type::boundary:
      if( possibleCollision )
      {
        callable(containedObject.Object(), containmentObject.Object());
      }
      break;
  }
}

inline auto geometry_containment::operator()(collision_object &containedObject, collision_object &containmentObject) -> bool
{
  auto containedPosition = containedObject.Object().Position();
  auto containmentPosition = containmentObject.Object().Position();

  auto containedBounds = containedObject.Bounds();
  auto containerBounds = containmentObject.Bounds();

  auto containedX = containerBounds.right > containedBounds.right && containerBounds.left < containedBounds.left;
  auto containedY = containerBounds.top < containedBounds.top && containerBounds.bottom > containedBounds.bottom;
  auto possibleCollision = containedX && containedY;

  switch( m_type )
  {
    case collision_type::direct2d:
      return possibleCollision && CheckDirect2D(containedObject, containmentObject);

    case collision_type::boundary:
    default:
      return possibleCollision;

  }
}
