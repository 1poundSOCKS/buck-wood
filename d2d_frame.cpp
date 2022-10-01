#include "d2d_frame.h"

d2d_frame::d2d_frame(const winrt::com_ptr<ID2D1HwndRenderTarget>& renderTarget, int gameScreenWidth, int gameScreenHeight)
 : renderTarget(renderTarget), gameScreenWidth(gameScreenWidth), gameScreenHeight(gameScreenHeight)
{
  HRESULT hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f)), brush.put());
  if( FAILED(hr) ) throw(L"error");

  hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(writeFactory),reinterpret_cast<IUnknown**>(writeFactory.put()));
  if( FAILED(hr) ) throw L"error";

  hr = writeFactory->CreateTextFormat(L"Verdana",NULL,DWRITE_FONT_WEIGHT_NORMAL,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,20,L"", writeTextFormat.put());
  if( FAILED(hr) ) throw L"error";

  hr = writeTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
  if( FAILED(hr) ) throw L"error";

  hr = writeTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
  if( FAILED(hr) ) throw L"error";

  D2D1_SIZE_F frameSize = renderTarget->GetSize();
  scaleX = static_cast<double>(frameSize.width) / static_cast<double>(gameScreenWidth);
  scaleY = static_cast<double>(frameSize.height) / static_cast<double>(gameScreenHeight);

  D2D1_SIZE_F size;
  size.width = scaleX;
  size.height = scaleY;
  scale = std::make_unique<D2D1::Matrix3x2F>(D2D1::Matrix3x2F::Scale(size));

  renderTarget->BeginDraw();
}

d2d_frame::~d2d_frame()
{
  renderTarget->EndDraw();
}
