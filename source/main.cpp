#include "pch.h"

#include "framework.h"
#include "main_menu_screen.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"D3D11.lib")
#pragma comment(lib,"d2d1")
#pragma comment(lib,"Dwrite")
#pragma comment(lib,"Dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"gtest.lib")
#pragma comment(lib,"gtest_main.lib")
#pragma comment(lib,"RuntimeObject.lib")
#pragma comment(lib,"jsoncpp.lib")

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
  main_menu_screen mainMenuScreen;

  framework::create(instance, cmdShow);
  framework::openScreen(mainMenuScreen);

  return 0;
}
