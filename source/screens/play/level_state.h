#ifndef _level_state_
#define _level_state_

#include "game_level_data.h"
#include "player_ship.h"
#include "bullet.h"
#include "screen_input_state.h"
#include "screen_render_data.h"
#include "level_control_state.h"
#include "area_grid.h"
#include "explosion.h"
#include "level_target.h"
#include "level_island.h"
#include "active_object.h"
#include "play_event.h"
#include "starfield.h"
#include "sound_data.h"

struct level_state
{
public:

  using active_object_collection_type = std::list<active_object>;

  level_state(int64_t counterFrequency);
  [[nodiscard]] auto GetActiveObjectInserter() -> std::back_insert_iterator<active_object_collection_type>;
  auto Update(int64_t counterValue) -> void;
  auto RenderTo(ID2D1RenderTarget* renderTarget, const D2D1::Matrix3x2F& viewTransform) -> void;
  [[nodiscard]] auto IsComplete() -> bool;
  [[nodiscard]] auto GetUpdateInterval() -> float;

private:

  [[nodiscard]] auto GetViewRect(ID2D1RenderTarget* renderTarget, const D2D1::Matrix3x2F& viewTransform) -> D2D1_RECT_F;

  const int64_t counterFrequency = 0;
  int64_t currentTimerCount = 0;
  int64_t previousTimerCount = 0;
  active_object_collection_type m_activeObjects;
};

#endif
