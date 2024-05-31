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

  // auto containedX = containedBounds.left < containerBounds.right && containedBounds.right > containerBounds.left;
  // auto containedY = containedBounds.top < containerBounds.bottom && containedBounds.bottom > containerBounds.top;
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

// class geometry_containment
// {

// public:

//   geometry_containment()
//   {
//   }

//   [[nodiscard]] auto IsContained(ID2D1Geometry* containmentGeometry, ID2D1Geometry* geometry) const noexcept -> bool
//   {
//     D2D1_GEOMETRY_RELATION relation = D2D1_GEOMETRY_RELATION_UNKNOWN;
//     HRESULT hr = containmentGeometry->CompareWithGeometry(geometry, D2D1::Matrix3x2F::Identity(), &relation);

//     bool contained = false;

//     if( SUCCEEDED(hr) )
//     {
//       switch( relation )
//       {
//         case D2D1_GEOMETRY_RELATION_CONTAINS:
//           contained = true;
//           break;
//       }
//     }

//     return contained;
//   }

//   auto operator()(ID2D1Geometry* containmentGeometry, auto&& containedObject, auto&& callable) -> void
//   {
//     switch( game_settings::collisionDetectionType() )
//     {
//       case game_settings::collision_detection_type::direct2d:
//         if( CheckDirect2D(containmentGeometry, containedObject) )
//         {
//           std::lock_guard<std::mutex> guard(m_mutex);
          
//           if( !containedObject.Object().Destroyed() )
//           {
//             callable(containedObject.Object());
//           }
//         }
//         break;
//       case game_settings::collision_detection_type::basic:
//         if( !direct2d::CheckOverlap(direct2d::GetGeometryBounds(containmentGeometry), containedObject.GeometryBounds()) )
//         {
//           std::lock_guard<std::mutex> guard(m_mutex);
//           if( !containedObject.Object().Destroyed() )
//           {
//             callable(containedObject.Object());
//           }
//         }
//         break;
//     }
//   }

//   auto CheckDirect2D(ID2D1Geometry* containmentGeometry, auto&& containedObject) -> bool
//   {
//     D2D1_GEOMETRY_RELATION relation = D2D1_GEOMETRY_RELATION_UNKNOWN;
//     HRESULT hr = containmentGeometry->CompareWithGeometry(containedObject.Geometry(), D2D1::Matrix3x2F::Identity(), &relation);

//     bool contained = false;

//     if( SUCCEEDED(hr) )
//     {
//       switch( relation )
//       {
//         case D2D1_GEOMETRY_RELATION_CONTAINS:
//           contained = true;
//           break;
//       }
//     }

//     return !contained;
//   }

//   auto operator()(ID2D1Geometry* containmentGeometry, std::ranges::input_range auto&& objectCollection, auto&& callable) -> void
//   {
//     std::for_each(std::execution::par, std::begin(objectCollection), std::end(objectCollection), [this,containmentGeometry,&callable](auto& object)
//     {
//       (*this)(containmentGeometry, object, callable);
//     });
//   }

// private:

//   std::mutex m_mutex;

// };
