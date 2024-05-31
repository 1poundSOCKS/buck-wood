#include "pch.h"
#include "level_containment_handler.h"

auto level_containment_handler::operator()(default_object &object1, default_object &object2) -> void
{
  OnContainment<player_ship, portal>(object1, object2);
}

auto level_containment_handler::OnContainment(player_ship &player, portal &portalObj) -> void
{
  m_exitCell = portalObj.CellId();
}
