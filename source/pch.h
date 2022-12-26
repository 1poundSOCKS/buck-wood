#define WIN32_LEAN_AND_MEAN
#define UNICODE
#define DIRECTINPUT_VERSION 0x0800

#include <iostream>
#include <tchar.h>
#include <string>
#include <string_view>
#include <math.h>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <numeric>
#include <format>
#include <limits>
#include <random>
#include <cmath>

#include <windows.h>
#include <windowsx.h>
#include <winrt/base.h>
#include <mmsystem.h>
#include <codecvt>

#include <dxgi.h>
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>
#include <dinput.h>
#include <dsound.h>

#include "json.h"

namespace fs = std::filesystem;

#define ENABLE_MUSIC
