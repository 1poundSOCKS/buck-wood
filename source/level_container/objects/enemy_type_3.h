#pragma once

#include "enemy_object.h"
#include "reload_timer.h"

class enemy_type_3 : public enemy_object
{

public:

  enemy_type_3(POINT_2F position, int hitpoints, float reloadTime);

  auto Update(float interval) noexcept -> void;

private:

  reload_timer m_reloadTimer;

};
