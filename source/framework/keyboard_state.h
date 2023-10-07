#pragma once

class keyboard_state
{
public:

  keyboard_state();

  auto Update(IDirectInputDevice8* keyboard) -> void;
  auto operator=(const keyboard_state& state) -> const keyboard_state&;
  [[nodiscard]] auto Down(int key) const -> bool;

private:

  unsigned char m_data[256];

};

inline keyboard_state::keyboard_state()
{
  ::ZeroMemory(m_data, sizeof(m_data));
}

inline auto keyboard_state::Update(IDirectInputDevice8* keyboard) -> void
{
  HRESULT hr = keyboard->GetDeviceState(sizeof(m_data), m_data);

	if(FAILED(hr))
	{
		if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) keyboard->Acquire();
	}
}

inline auto keyboard_state::operator=(const keyboard_state& state) -> const keyboard_state&
{
  ::memcpy(m_data, state.m_data, sizeof(m_data));
  return *this;
}

[[nodiscard]] inline auto keyboard_state::Down(int key) const -> bool
{
  return m_data[key] & 0x80 ? true : false;
}
