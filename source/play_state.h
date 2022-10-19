#ifndef _play_state_
#define _play_state_

#include "game_objects.h"
#include "game_level.h"
#include "control_state.h"
#include "game_math.h"
#include "game_events.h"
#include "system_timer.h"

struct play_state
{
  play_state(const system_timer& timer, const game_level_ptr& firstLevel);

  enum PLAYER_STATE { player_alive, player_dead };
  enum LEVEL_STATE { level_incomplete, level_complete };

  const system_timer& timer;
  game_level_ptr currentLevel;
  LEVEL_STATE levelState = level_incomplete;

  PLAYER_STATE playerState = player_alive;
  std::unique_ptr<player_ship> player;
  std::list<std::unique_ptr<bullet>> bullets;

  int64_t levelTimerStart;
  int64_t levelTimerStop = 0;
};

using play_state_ptr = std::unique_ptr<play_state>;

game_events_ptr UpdatePlayState(play_state& playState, const control_state& controlState, float gameUpdateInterval);
void UpdatePlayer(play_state& playState, const control_state& controlState, float gameUpdateInterval);
void UpdateBullets(play_state& playState, const control_state& controlState, float gameUpdateInterval, game_events& events);
bool PlayerIsOutOfBounds(const play_state& playState);
play_state::LEVEL_STATE GetLevelState(const play_state& playState);

#endif
