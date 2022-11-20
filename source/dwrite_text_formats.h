#ifndef _dwrite_text_formats_
#define _dwrite_text_formats_

// #include <memory>
// #include <winrt/base.h>
// #include <dwrite.h>

struct dwrite_text_formats
{
  dwrite_text_formats();

  winrt::com_ptr<IDWriteFactory> writeFactory;
  winrt::com_ptr<IDWriteTextFormat> writeTextFormat;
  winrt::com_ptr<IDWriteTextFormat> menuTextFormat;
  winrt::com_ptr<IDWriteTextFormat> levelTimerTextFormat;
  winrt::com_ptr<IDWriteTextFormat> levelEndTextFormat;
};

using dwrite_text_formats_ptr = std::shared_ptr<dwrite_text_formats>;

#endif
