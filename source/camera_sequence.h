#pragma once

class camera_sequence
{

public:

  struct camera_position
  {
    float x;
    float y;
    float scale;
  };

  camera_sequence();
  camera_sequence(camera_position position);
  auto AddPause(int64_t ticks) -> void;
  auto AddMove(camera_position position, int64_t ticks) -> void;
  [[nodiscard]] auto GetPosition(int64_t ticks) const -> camera_position;
  [[nodiscard]] auto GetScale(int64_t ticks) const -> float;
  [[nodiscard]] auto GetTotalTicks() const -> int64_t;

private:

  struct camera_move
  {
    camera_position position;
    int64_t ticks;
  };

  using camera_move_collection = std::vector<camera_move>;
  camera_move_collection m_moves;
};
