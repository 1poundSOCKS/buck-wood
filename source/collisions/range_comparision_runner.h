#pragma once

class range_comparison_runner
{

public:

  enum class execution { sequential, parallel };

  range_comparison_runner(execution ex);
  
  auto operator()(std::ranges::input_range auto& objectCollection1, std::ranges::input_range auto& objectCollection2, auto&& callable) -> void;

private:

  execution m_execution { execution::sequential };
  std::mutex m_mutex;

};

inline range_comparison_runner::range_comparison_runner(execution ex) : m_execution { ex }
{
}

auto range_comparison_runner::operator()(std::ranges::input_range auto& objectCollection1, std::ranges::input_range auto& objectCollection2, auto &&callable) -> void
{
  switch( m_execution )
  {
    case execution::sequential:
      std::for_each(std::execution::seq, std::begin(objectCollection1), std::end(objectCollection1), [this,&objectCollection2,&callable](auto& object1)
      {
        for( auto& object2 : objectCollection2 )
        {
          callable(object1, object2);
        }
      });
      break;
    case execution::parallel:
      std::for_each(std::execution::par, std::begin(objectCollection1), std::end(objectCollection1), [this,&objectCollection2,&callable](auto& object1)
      {
        for( auto& object2 : objectCollection2 )
        {
          std::lock_guard<std::mutex> guard(m_mutex);
          callable(object1, object2);
        }
      });
      break;
  }
}
