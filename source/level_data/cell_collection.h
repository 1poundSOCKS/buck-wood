#pragma once

#include "cell_id.h"
#include "level_types.h"

class cell_collection
{

public:

  cell_collection() noexcept;

  auto Set(cell_id cellId, level_cell_type cellType) noexcept -> void;
  auto Reset() noexcept -> void;

  [[nodiscard]] auto GetType(cell_id cellId) const noexcept -> level_cell_type;
  [[nodiscard]] auto IsTypeOf(cell_id cellId, level_cell_type cellType) const noexcept -> bool;
  auto Enumerate(auto&& visitor) const noexcept -> void;

private:

  using key_type = cell_id;
  using map_entry_type = std::pair<const key_type, level_cell_type>;
  using cell_allocator_type = custom_allocator<map_entry_type>;
  using collection_allocator_type = custom_allocator<map_entry_type>;
  using collection_type = std::map<key_type, level_cell_type, std::less<key_type>, collection_allocator_type>;

private:

  [[nodiscard]] auto GetType(collection_type::const_iterator cellEntry) const noexcept -> level_cell_type;

private:

  custom_allocator_state m_cellBuffer;
  cell_allocator_type m_cellAllocator;
  collection_type m_cells;

};

auto cell_collection::Enumerate(auto &&visitor) const noexcept -> void
{
  for( const auto& cellEntry : m_cells )
  {
    const auto& [cellId, cellType] = cellEntry;
    visitor(cellId, cellType);
  }
}
