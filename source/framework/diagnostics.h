#ifndef _diagnostics_
#define _diagnostics_

// #include "framework.h"
#include "screen_input_state.h"

using diagnostics_data_collection = std::vector<std::wstring>;
using diagnostics_data_const_iterator = diagnostics_data_collection::const_iterator;
using diagnostics_data_inserter_type = std::back_insert_iterator<diagnostics_data_collection>;

void FormatDiagnostics(const screen_input_state& inputState, diagnostics_data_inserter_type diagnosticsDataInserter);
std::wstring GetDiagnosticsString(diagnostics_data_const_iterator textBegin, diagnostics_data_const_iterator textEnd);

#endif
