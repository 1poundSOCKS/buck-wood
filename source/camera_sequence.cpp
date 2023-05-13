#include "pch.h"
#include "camera_sequence.h"
#include "performance_counter.h"

camera_sequence::camera_sequence(camera_position position)
{
  m_moves.emplace_back(position);
}

auto camera_sequence::AddPause(int64_t ticks) -> void
{
  m_moves.emplace_back( camera_move { m_moves.back().position, ticks } );
}

auto camera_sequence::AddMove(camera_position position, int64_t ticks) -> void
{
  m_moves.emplace_back( camera_move { position, ticks } );
}

[[nodiscard]] auto camera_sequence::GetPosition(int64_t ticks) const -> camera_position
{
  auto cameraMove = m_moves.cbegin();
  auto remainingTicks = ticks;
  
  while( cameraMove != m_moves.cend() && remainingTicks > cameraMove->ticks )
  {
    remainingTicks -= cameraMove->ticks;
    ++cameraMove;
  }

  if( cameraMove == m_moves.cend() )
  {
    return { m_moves.back().position.x, m_moves.back().position.y, m_moves.back().position.scale };
  }
  else if( cameraMove == m_moves.cbegin() )
  {
    return { m_moves.front().position.x, m_moves.front().position.y, m_moves.front().position.scale };
  }
  else
  {
    auto previousMove = std::prev(cameraMove);

    auto cx = cameraMove->position.x - previousMove->position.x;
    auto cy = cameraMove->position.y - previousMove->position.y;
    auto cScale = cameraMove->position.scale - previousMove->position.scale;

    auto x = cx * remainingTicks / cameraMove->ticks + previousMove->position.x;
    auto y = cy * remainingTicks / cameraMove->ticks + previousMove->position.y;
    auto scale = cScale * remainingTicks / cameraMove->ticks + previousMove->position.scale;

    return { x, y, scale };
  }
}

[[nodiscard]] auto camera_sequence::GetTotalTicks() const -> int64_t
{
  return std::accumulate(m_moves.cbegin(), m_moves.cend(), 0i64, [](int64_t total, const camera_move& move) -> int64_t { return total + move.ticks; });
}
