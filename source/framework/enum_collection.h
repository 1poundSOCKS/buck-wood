#pragma once

template <typename enum_type, typename object_type>
class enum_collection
{

public:

  using initializer_type = std::tuple<enum_type, object_type>;
  using collection_type = std::vector<object_type>;

  enum_collection(std::ranges::input_range auto&& items);
  [[nodiscard]] auto operator[](enum_type item) const -> const object_type&;

private:

  collection_type m_items;

};

template <typename enum_type, typename object_type>
enum_collection<enum_type, object_type>::enum_collection(std::ranges::input_range auto&& items)
{
  for( const auto& [item, object] : items )
  {
    auto index = static_cast<std::underlying_type<enum_type>::type>(item);
    auto requiredSize = index + 1;

    if( m_items.size() < requiredSize )
    {
      m_items.resize(requiredSize);
    }

    m_items[index] = object;
  }
}

template <typename enum_type, typename object_type>
[[nodiscard]] auto enum_collection<enum_type, object_type>::operator[](enum_type item) const -> const object_type&
{
  auto index = static_cast<std::underlying_type<enum_type>::type>(item);
  return m_items[index];
}
