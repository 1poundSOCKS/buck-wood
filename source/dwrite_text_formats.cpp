#include "dwrite_text_formats.h"

dwrite_text_formats::dwrite_text_formats()
{
  HRESULT hr = S_OK;
  
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
}
