#ifndef _diagnostics_
#define _diagnostics_

#include "framework.h"
#include "screen_input_state.h"

void FormatDiagnostics(const screen_input_state& inputState, auto diagnosticsDataInserter)
{
  static wchar_t text[64];
  swprintf(text, L"mouse: %i,%i", inputState.windowData.mouse.x, inputState.windowData.mouse.y);
  diagnosticsDataInserter = text;
  swprintf(text, L"client rect: %i,%i", inputState.windowData.clientRect.right, inputState.windowData.clientRect.bottom);
  diagnosticsDataInserter = text;
}

std::wstring GetDiagnosticsString(auto textBegin, auto textEnd)
{
  return std::reduce(textBegin, textEnd, std::wstring(L""), [](const auto& complete, const auto& value)
  {
    return complete + value + L'\n';
  });
}

#endif
