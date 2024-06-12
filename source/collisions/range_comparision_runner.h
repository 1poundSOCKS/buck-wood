#pragma once

#include "default_object.h"

class range_comparison_runner
{

public:

  auto operator()(std::ranges::input_range auto& objectCollection1, std::ranges::input_range auto& objectCollection2, auto&& callable) -> void;

private:

  std::mutex m_mutex;

};

auto range_comparison_runner::operator()(std::ranges::input_range auto& objectCollection1, std::ranges::input_range auto& objectCollection2, auto &&callable) -> void
{
  std::for_each(std::execution::par, std::begin(objectCollection1), std::end(objectCollection1), [this,&objectCollection2,&callable](default_object_geometry& object1)
  {
    // for( auto& object2 : objectCollection2 )
    for( default_object_geometry& object2 : objectCollection2 )
    {
      std::lock_guard<std::mutex> guard(m_mutex);
      callable(object1, object2);
    }
  });
}
