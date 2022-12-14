#ifndef _data_files_
#define _data_files_

struct config_file
{
  struct setting
  {
    setting(const std::wstring& text);

    std::wstring key;
    std::wstring value;
  };

  config_file(const wchar_t* filename);

  std::map<std::wstring, std::wstring> settings;
};

struct wav_file_header
{
  wav_file_header(std::ifstream& stream);

  uint8_t id[4];
  uint32_t size;
  uint8_t format[4];
};

struct wav_file_data_format
{
  unsigned short audioFormat;
  unsigned short numChannels;
  unsigned long sampleRate;
  unsigned long bytesPerSecond;
  unsigned short blockAlign;
  unsigned short bitsPerSample;
};

struct wav_file_chunk
{
  wav_file_chunk(std::ifstream& stream);

  uint8_t id[4];
  uint32_t size;
  std::unique_ptr<uint8_t[]> data;
};

struct wav_file_data
{
  wav_file_data(const wchar_t* filename);

  std::unique_ptr<wav_file_header> header;
  std::unique_ptr<wav_file_chunk> format;
  std::unique_ptr<wav_file_chunk> data;
  wav_file_data_format dataFormat;
};

int GetWavFileDataSize(const wav_file_data& data);

using wav_file_data_ptr = std::unique_ptr<wav_file_data>;

#endif
