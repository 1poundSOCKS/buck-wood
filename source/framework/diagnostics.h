#ifndef _diagnostics_
#define _diagnostics_

#include "framework.h"
#include "screen_input_state.h"

void FormatDiagnostics(const screen_input_state& inputState, auto diagnosticsDataInserter)
{
  diagnosticsDataInserter = std::format(L"mouse: {}, {}", inputState.windowData.mouse.x, inputState.windowData.mouse.y);
  diagnosticsDataInserter = std::format(L"client rect: {}, {}", inputState.windowData.clientRect.right, inputState.windowData.clientRect.bottom);
}

std::wstring GetDiagnosticsString(auto textBegin, auto textEnd)
{
  return std::reduce(textBegin, textEnd, std::wstring(L""), [](const auto& complete, const auto& value)
  {
    return complete + value + L'\n';
  });
}

#endif
