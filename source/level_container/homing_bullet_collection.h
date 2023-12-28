#pragma once

class homing_bullet_collection
{

public:

  auto UpdateBullets(float interval, std::ranges::input_range auto&& bullets) -> void
  {
    for( auto& bullet : bullets )
    {
      bullet.Update(interval, std::nullopt);
    }
  }

};
