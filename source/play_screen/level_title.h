#pragma once

class level_title
{

public:

  level_title(int index) : m_index { index }
  {
  }

  [[nodiscard]] auto Value() const -> std::wstring
  {
    return std::format(L"Wave {:02d}", m_index);
  }

private:

  int m_index;

};
