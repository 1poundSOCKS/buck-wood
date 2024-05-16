#include "pch.h"
#include "enemy _type_3.h"

enemy_type_3::enemy_type_3(POINT_2F position, int hitpoints, float reloadTime) :
  enemy_object { position, { 1.5, 1.5 }, 0, hitpoints }, m_reloadTimer { reloadTime }
{
}
