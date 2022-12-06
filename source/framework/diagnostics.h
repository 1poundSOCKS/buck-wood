#ifndef _diagnostics_
#define _diagnostics_

#include "framework.h"
#include "perf_data.h"
#include "timers.h"
#include "screen_input_state.h"

using diagnostics_data = std::vector<std::wstring>;

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const perf_data& perfData, const system_timer& timer);
void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const screen_input_state& inputState);
std::wstring GetDiagnosticsString(diagnostics_data::const_iterator begin, diagnostics_data::const_iterator end);

#endif
