#include "drag_drop.h"
#include <numeric>
#include "game_math.h"

drag_drop_point::drag_drop_point(float x, float y) : x(x), y(y)
{
}

void SelectClosestPoint(drag_drop_shape& shape, float x, float y)
{
  drag_drop_shape::iterator currentPoint = shape.closestPoint = shape.points.begin();
  
  if( shape.points.size() == 0 ) return;

  float closestDistance = GetDistanceBetweenPoints(x, y, shape.closestPoint->x, shape.closestPoint->y);
  
  while( ++currentPoint != shape.points.end() )
  {
    float currentDistance = GetDistanceBetweenPoints(x, y, currentPoint->x, currentPoint->y);
    if( currentDistance < closestDistance )
    {
      shape.closestPoint = currentPoint;
      closestDistance = currentDistance;      
    }
  }
}

void ProcessDragDrop(drag_drop_state& dragDropState, const drag_drop_control_state& controlState)
{
  if( !controlState.leftMouseButtonDown )
  {
    dragDropState.grabbedPoint = dragDropState.shape.points.end();
    SelectClosestPoint(dragDropState.shape, controlState.worldMouseX, controlState.worldMouseY);
    dragDropState.highlightedPoint = dragDropState.shape.closestPoint;
  }

  bool drag = false;
  if( dragDropState.highlightedPoint != dragDropState.shape.points.end() && controlState.leftMouseButtonDown )
  {
    dragDropState.grabbedPoint = dragDropState.highlightedPoint;
    dragDropState.highlightedPoint = dragDropState.shape.points.end();
  }

  if( dragDropState.grabbedPoint != dragDropState.shape.points.end() && controlState.leftMouseButtonDrag )
  {
    dragDropState.grabbedPoint->x = controlState.worldMouseX;
    dragDropState.grabbedPoint->y = controlState.worldMouseY;
  }
}
