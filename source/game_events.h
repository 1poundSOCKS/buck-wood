#ifndef _game_events_
#define _game_events_

#include <memory>

struct game_events
{
  bool targetShot = false;
  bool startTitleMusic = false;
  bool stopTitleMusic = false;
  bool playerShot = false;
  bool playerBoosterOn = false;
  bool playerBoosterOff = false;
};

using game_events_ptr = std::shared_ptr<game_events>;

#endif
