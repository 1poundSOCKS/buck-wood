#ifndef _diagnostics_
#define _diagnostics_

#include <vector>
#include <string>
#include "control_state.h"
#include "perf_data.h"
#include "timers.h"

using diagnostics_data = std::vector<std::wstring>;

void FormatDiagnostics(diagnostics_data& diagnosticsData, const control_state& controlState, const perf_data& perfData, const system_timer& timer);

#endif
