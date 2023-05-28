#pragma once

#include "level_target.h"

template <typename item_type>
class target_container
{

public:

  using collection = std::vector<item_type>;

  target_container(const collection& items);
  auto operator()() const;

private:

  const collection& m_items;

};

template <typename item_type>
target_container<item_type>::target_container(const collection& items) : m_items(items)
{
}

template <typename item_type>
auto target_container<item_type>::operator()() const
{
  return m_items();
}
