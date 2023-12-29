#pragma once

namespace particle_functions
{

  template <typename...Args>auto update(std::ranges::input_range auto&& objects, Args...args) -> void
  {
    for( auto& object : objects )
    {
      object.Update(std::forward<Args>(args)...);
    }
  }

  auto erase_destroyed(std::ranges::input_range auto&& objects) -> void
  {
    std::erase_if(objects, [](const auto& object) -> bool { return object.Destroyed(); });
  }

}
