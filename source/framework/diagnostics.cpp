#include "pch.h"
#include "diagnostics.h"

void FormatDiagnostics(const screen_input_state& inputState, diagnostics_data_inserter_type diagnosticsDataInserter)
{
  diagnosticsDataInserter = std::format(L"mouse: {}, {}", inputState.windowData.mouse.x, inputState.windowData.mouse.y);
  diagnosticsDataInserter = std::format(L"client rect: {}, {}", inputState.windowData.clientRect.right, inputState.windowData.clientRect.bottom);
}

std::wstring GetDiagnosticsString(diagnostics_data_const_iterator textBegin, diagnostics_data_const_iterator textEnd)
{
  return std::reduce(textBegin, textEnd, std::wstring(L""), [](const auto& complete, const auto& value)
  {
    return complete + value + L'\n';
  });
}
