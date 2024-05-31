#pragma once

class range_comparison_runner
{

public:

  auto operator()(std::ranges::input_range auto&& objectCollection1, std::ranges::input_range auto&& objectCollection2, auto&& callable) -> void;

private:

  std::mutex m_mutex;

};

auto range_comparison_runner::operator()(std::ranges::input_range auto &&objectCollection1, std::ranges::input_range auto &&objectCollection2, auto &&callable) -> void
{
  std::for_each(std::execution::par, std::begin(objectCollection1), std::end(objectCollection1), [this,&objectCollection2,&callable](auto& object1)
  {
    for( auto& object2 : objectCollection2 )
    {
      std::lock_guard<std::mutex> guard(m_mutex);
      callable(object1, object2);
    }
  });
}
