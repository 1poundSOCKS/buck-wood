#ifndef _diagnostics_
#define _diagnostics_

#include "screen_input_state.h"
#include "frame_data.h"
#include "text_renderer.h"

using diagnostics_data_collection = std::vector<std::wstring>;
using diagnostics_data_const_iterator = diagnostics_data_collection::const_iterator;
using diagnostics_data_inserter_type = std::back_insert_iterator<diagnostics_data_collection>;

void FormatDiagnostics(const screen_input_state& inputState, diagnostics_data_inserter_type diagnosticsDataInserter);
std::wstring GetDiagnosticsString(diagnostics_data_const_iterator textBegin, diagnostics_data_const_iterator textEnd);

class diagnostics
{

public:

  static auto create() -> void;
  static auto destroy() -> void;

  static auto add(std::wstring_view label, auto value) -> void;
  static auto addTime(std::wstring_view label, int64_t ticks) -> void;
  static auto add(const screen_input_state& screenInputState) -> void;
  static auto setUpdateTime(int64_t ticks) -> void;
  static auto setRenderTime(int64_t ticks) -> void;
  static auto addTimingData() -> void;
  static auto updateFrameData() -> void;
  static auto clear() -> void;

  [[nodiscard]] static auto data() -> const diagnostics_data_collection&;
  [[nodiscard]] static auto text() -> std::wstring;

private:

  auto AddTime(std::wstring_view label, int64_t ticks) -> void;
  auto Add(const screen_input_state& screenInputState) -> void;
  auto AddTimingData() -> void;
  [[nodiscard]] auto Text() const -> std::wstring;

private:

  static diagnostics* m_instance;

  diagnostics_data_collection m_diagnosticsData;
  int64_t m_diagnosticsUpdateTime { 0 };
  int64_t m_diagnosticsRenderTime { 0 };
  frame_data m_frameData;

};

inline auto diagnostics::add(std::wstring_view label, auto value) -> void
{
  m_instance->m_diagnosticsData.emplace_back(std::format(L"{}: {}", label, value));
}

inline auto diagnostics::addTime(std::wstring_view label, int64_t ticks) -> void
{
  m_instance->AddTime(label, ticks);
}

inline auto diagnostics::add(const screen_input_state& screenInputState) -> void
{
  m_instance->Add(screenInputState);
}

inline auto diagnostics::setUpdateTime(int64_t ticks) -> void
{
  m_instance->m_diagnosticsUpdateTime = ticks;
}

inline auto diagnostics::setRenderTime(int64_t ticks) -> void
{
  m_instance->m_diagnosticsRenderTime = ticks;
}

inline auto diagnostics::addTimingData() -> void
{
  m_instance->AddTimingData();
}

inline auto diagnostics::updateFrameData() -> void
{
  m_instance->m_frameData.Update();
}

inline auto diagnostics::clear() -> void
{
  m_instance->m_diagnosticsData.clear();
}

[[nodiscard]] inline auto diagnostics::data() -> const diagnostics_data_collection&
{
  return m_instance->m_diagnosticsData;
}

[[nodiscard]] inline auto diagnostics::text() -> std::wstring
{
  return m_instance->Text();
}

#endif
