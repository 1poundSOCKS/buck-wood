#include "d2d_frame.h"

d2d_frame::d2d_frame(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, float yOffset)
 : renderTarget(renderTarget)
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

  transform = D2D1::Matrix3x2F::Translation(0, yOffset);

  renderTarget->BeginDraw();
}

d2d_frame::~d2d_frame()
{
  renderTarget->EndDraw();
}

std::unique_ptr<game_point> GetMousePositionInFrame(float mouseX, float mouseY, const d2d_frame& frame)
{
  std::unique_ptr<game_point> mousePosition = std::make_unique<game_point>();

  // D2D1_SIZE_F renderTargetSize = frame.renderTarget->GetSize();
  // float mouseX = mouseX * renderTargetSize.width;
  // float mouseY = mouseY * renderTargetSize.height;

  // D2D1_POINT_2F inPoint;
  // inPoint.x = mouseX;
  // inPoint.y = mouseY;
  // D2D1_POINT_2F outPoint = frame.transform.TransformPoint(inPoint);

  return mousePosition;
}
