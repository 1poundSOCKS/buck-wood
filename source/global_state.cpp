#include "global_state.h"

global_state::global_state(const d2d_app& app, const std::wstring& dataPath)
: dataPath(dataPath), 
  brushes(app.d2d_rendertarget), 
  renderBrushes(app.d2d_rendertarget),
  soundBuffers(app.directSound, dataPath)
{
  gameLevelDataIndex = LoadAllGameLevelData(dataPath);
}
