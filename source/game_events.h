#ifndef _game_events_
#define _game_events_

#include <memory>

struct game_events
{
  bool playerShot = false;
  bool targetShot = false;
};

using game_events_ptr = std::shared_ptr<game_events>;

#endif
