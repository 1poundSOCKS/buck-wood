#pragma once

template<typename... Ts> struct make_overload: Ts... { using Ts::operator()...; };
template<typename... Ts> make_overload(Ts...) -> make_overload<Ts...>;
