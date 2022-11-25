#ifndef _diagnostics_
#define _diagnostics_

#include "framework.h"
#include "control_state.h"
#include "global_state.h"

using diagnostics_data = std::vector<std::wstring>;

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const perf_data& perfData, const system_timer& timer);
void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const global_state& globalState);
void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const control_state& controlState);

#endif
