#include "d2d_frame.h"

d2d_frame::d2d_frame(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget)
 : renderTarget(renderTarget)
{
  HRESULT hr = S_OK;
  
  hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f)), brush.put());
  if( FAILED(hr) ) throw(L"error");

  hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(1.0f, 0.0f, 0.0f, 0.5f)), brushThrusters.put());
  if( FAILED(hr) ) throw(L"error");

  hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f)), brushDiagnostics.put());
  if( FAILED(hr) ) throw(L"error");

  hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f)), brushTimer.put());
  if( FAILED(hr) ) throw(L"error");

  hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f)), brushDeactivated.put());
  if( FAILED(hr) ) throw(L"error");

  hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f)), brushActivated.put());
  if( FAILED(hr) ) throw(L"error");

  hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(0.0f, 1.0f, 1.0f, 1.0f)), brushLevelEndText.put());
  if( FAILED(hr) ) throw(L"error");

  hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(writeFactory),reinterpret_cast<IUnknown**>(writeFactory.put()));
  if( FAILED(hr) ) throw L"error";

  hr = writeFactory->CreateTextFormat(L"Verdana",NULL,DWRITE_FONT_WEIGHT_NORMAL,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,20,L"", writeTextFormat.put());
  if( FAILED(hr) ) throw L"error";

  hr = writeTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
  if( FAILED(hr) ) throw L"error";

  hr = writeFactory->CreateTextFormat(L"Verdana",NULL,DWRITE_FONT_WEIGHT_NORMAL,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,40,L"", menuTextFormat.put());
  if( FAILED(hr) ) throw L"error";

  hr = menuTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
  if( FAILED(hr) ) throw L"error";

  hr = menuTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
  if( FAILED(hr) ) throw L"error";

  hr = writeFactory->CreateTextFormat(L"Aldhabi",NULL,DWRITE_FONT_WEIGHT_BOLD,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,60,L"", levelTimerTextFormat.put());
  if( FAILED(hr) ) throw L"error";

  hr = writeFactory->CreateTextFormat(L"Aldhabi",NULL,DWRITE_FONT_WEIGHT_BOLD,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,100,L"", levelEndTextFormat.put());
  if( FAILED(hr) ) throw L"error";

  hr = levelEndTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
  if( FAILED(hr) ) throw L"error";

  hr = levelEndTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
  if( FAILED(hr) ) throw L"error";

  renderTarget->BeginDraw();
}

d2d_frame::~d2d_frame()
{
  renderTarget->EndDraw();
}
