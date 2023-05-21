#pragma once

#include "passive_object.h"
#include "object_input_data.h"

class passive_object_container
{
public:

  using collection_type = std::vector<passive_object>;
  using inserter_type = std::back_insert_iterator<collection_type>;

  passive_object_container();

  [[nodiscard]] auto GetInserter() -> inserter_type;
  [[nodiscard]] auto ObjectCount() -> size_t;

  template <typename object_type> auto operator+=(object_type&& object) -> void;

  auto Update(const object_input_data& inputData, int64_t elapsedTicks) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;
  auto Clear() -> void;

private:

  collection_type m_objects;
};

template <typename object_type> auto passive_object_container::operator+=(object_type&& object) -> void
{
  m_objects.emplace_back(object);
}
