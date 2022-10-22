#ifndef _play_state_
#define _play_state_

#include "game_objects.h"
#include "game_level.h"
#include "control_state.h"
#include "game_math.h"
#include "game_events.h"
#include "system_timer.h"
#include "state_objects.h"

game_events_ptr UpdatePlayState(game_state& gameState, const control_state& controlState, const system_timer& timer);
game_events_ptr UpdateGameState(game_state& gameState, const control_state& cs, const system_timer& timer);
void UpdatePlayer(play_state& playState, const control_state& controlState, const system_timer& timer);
void UpdateBullets(play_state& playState, const control_state& controlState, game_events& events, const system_timer& timer);
bool PlayerIsOutOfBounds(const play_state& playState);
play_state::LEVEL_STATE GetLevelState(const play_state& playState);
void ResetPlayState(play_state& playState, const system_timer& timer, const game_data_ptr& gameData);

#endif
