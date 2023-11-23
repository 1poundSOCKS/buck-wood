#include "pch.h"
#include "diagnostics.h"
#include "performance_counter.h"
#include "render_target.h"

auto GetPercentageTime(int64_t frameTicks, int64_t elapsedTime) -> float
{
  return static_cast<float>(elapsedTime) / static_cast<float>(frameTicks) * 100.0f;
}

diagnostics* diagnostics::m_instance { nullptr };

auto diagnostics::create() -> void
{
  destroy();
  m_instance = new diagnostics {};
}

auto diagnostics::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

auto diagnostics::AddTime(std::wstring_view label, int64_t ticks) -> void
{
  auto frameTime = performance_counter::QueryFrequency() / render_target::fps();
  m_diagnosticsData.emplace_back(std::format(L"{}: {:.1f}", label, GetPercentageTime(frameTime, m_diagnosticsUpdateTime)));
}

auto diagnostics::AddWindowData(const window_data& windowData) -> void
{
  auto inserter = std::back_inserter(m_diagnosticsData);
  inserter = std::format(L"mouse: {}, {}", windowData.mouse.x, windowData.mouse.y);
  inserter = std::format(L"client rect: {}, {}", windowData.clientRect.right, windowData.clientRect.bottom);
}

auto diagnostics::AddTimingData() -> void
{
  m_diagnosticsData.emplace_back(std::format(L"fps: {}", m_frameData.GetFPS()));
  auto frameTime = performance_counter::QueryFrequency() / render_target::fps();
  m_diagnosticsData.emplace_back(std::format(L"update time: {:.1f}", GetPercentageTime(frameTime, m_diagnosticsUpdateTime)));
  m_diagnosticsData.emplace_back(std::format(L"render time: {:.1f}", GetPercentageTime(frameTime, m_diagnosticsRenderTime)));
}

[[nodiscard]] auto diagnostics::Text() const -> std::wstring
{
  auto text = std::reduce(std::cbegin(m_diagnosticsData), std::cend(m_diagnosticsData), std::wstring(L""), [](const auto& complete, const auto& value)
  {
    return complete + value + L'\n';
  });

  return text;
}
