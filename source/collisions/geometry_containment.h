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

  [[nodiscard]] auto CheckDirect2D(collision_object &containedObject, collision_object &containmentObject) const noexcept -> bool;

private:

  collision_type m_type;

};

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
