#pragma once

template <typename interface_type>
class com_singleton
{

public:

  static auto destroy() -> void;

  [[nodiscard]] static auto get() -> const winrt::com_ptr<interface_type>&;
  [[nodiscard]] static auto get_raw() -> interface_type*;

protected:

  com_singleton(const winrt::com_ptr<interface_type>& comObject);

protected:

  inline static com_singleton<interface_type>* m_instance { nullptr };
  winrt::com_ptr<interface_type> m_comObject;

};

template <typename interface_type>
auto com_singleton<interface_type>::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

template <typename interface_type>
auto com_singleton<interface_type>::get() -> const winrt::com_ptr<interface_type>&
{
  return m_instance->m_comObject;
}

template <typename interface_type>
auto com_singleton<interface_type>::get_raw() -> interface_type*
{
  return m_instance->m_comObject.get();
}

template <typename interface_type>
com_singleton<interface_type>::com_singleton(const winrt::com_ptr<interface_type>& comObject) : m_comObject { comObject }
{
}
