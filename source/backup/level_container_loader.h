#pragma once

#include "level_container.h"

class level_container_loader
{

public:

  level_container_loader(std::unique_ptr<level_container> levelContainer);
  auto operator()(size_t column, size_t row, char cellType) -> void;
  operator std::unique_ptr<level_container>();

private:

  std::unique_ptr<level_container> m_levelContainer;

};

inline level_container_loader::level_container_loader(std::unique_ptr<level_container> levelContainer) : m_levelContainer { std::move(levelContainer) }
{
}

inline auto level_container_loader::operator()(size_t column, size_t row, char cellType) -> void
{
  auto columnIndex = static_cast<int>(column);
  auto rowIndex = static_cast<int>(row);
  
  switch( cellType )
  {
  case '1':
    m_levelContainer->AddFloorCell(columnIndex, rowIndex);
    m_levelContainer->CreateEnemyType1(POINT_2I { columnIndex, rowIndex }, 10);
    break;
  case '2':
    m_levelContainer->AddFloorCell(columnIndex, rowIndex);
    m_levelContainer->CreateEnemyType2(POINT_2I { columnIndex, rowIndex }, 3, 2.0f, 400.0f, 2.0f);
    break;
  case 'P':
    m_levelContainer->AddFloorCell(columnIndex, rowIndex);
    m_levelContainer->CreatePortal(POINT_2I { columnIndex, rowIndex });
    m_levelContainer->CreatePlayer(POINT_2I { columnIndex, rowIndex });
    break;
  case ' ':
    m_levelContainer->AddFloorCell(columnIndex, rowIndex);
    break;
  }
}

inline level_container_loader::operator std::unique_ptr<level_container>()
{
  return std::move(m_levelContainer);
}
