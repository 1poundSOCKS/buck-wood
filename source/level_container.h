#pragma once

#include "active_object_container.h"
#include "performance_counter.h"

class level_container
{
public:

  using timeout = std::function<void()>;

  auto SetTimeout(int time) -> void;

  [[nodiscard]] auto GetObjectContainer() const -> const active_object_container&;
  [[nodiscard]] auto GetObjectContainer() -> active_object_container&;

  auto Update(const object_input_data& inputData, int64_t ticks) -> void;

private:

  active_object_container m_objectContainer;

  int64_t m_timeoutTicks = 0;
  timeout m_timeoutEvent = []()->void{};

};
