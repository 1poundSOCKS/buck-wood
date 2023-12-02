#pragma once

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
  auto GetSize() const -> int;

  std::unique_ptr<wav_file_header> header;
  std::unique_ptr<wav_file_chunk> format;
  std::unique_ptr<wav_file_chunk> data;
  wav_file_data_format dataFormat;
};
