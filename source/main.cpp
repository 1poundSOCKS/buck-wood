#include "pch.h"

#include "framework.h"
#include "main_menu_screen.h"
#include "renderers.h"
#include "command_line.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"D3D11.lib")
#pragma comment(lib,"d2d1")
#pragma comment(lib,"Dwrite")
#pragma comment(lib,"Dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"RuntimeObject.lib")
#pragma comment(lib,"Shell32.lib")

#if 0
#pragma comment(lib,"gtest.lib")
#pragma comment(lib,"gtest_main.lib")
#endif

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
  framework::create(instance, cmdShow);
  framework::setGameSpeedMultiplier(2.0f);

  command_line commandLine { cmdLine };

  if( commandLine.Contains(L"-u") )
  {
    framework::unlockFrameRate();
  }

  if( !commandLine.Contains(L"-w") )
  {
    framework::fullScreen();
  }

  renderer::create();

  framework::openScreen<main_menu_screen>();

  renderer::destroy();
  framework::destroy();

  return 0;
}
