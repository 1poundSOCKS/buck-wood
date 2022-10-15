#include "game_data.h"

wav_file_header::wav_file_header(std::ifstream& reader)
{
  reader.read(reinterpret_cast<char*>(id), sizeof(id));
  reader.read(reinterpret_cast<char*>(&size), sizeof(size));
  reader.read(reinterpret_cast<char*>(format), sizeof(format));
}

wav_file_chunk::wav_file_chunk(std::ifstream& reader)
{
  reader.read(reinterpret_cast<char*>(id), sizeof(id));
  reader.read(reinterpret_cast<char*>(&size), sizeof(size));
  data = std::make_unique<uint8_t[]>(size);
  reader.read(reinterpret_cast<char*>(data.get()), size);
}

wav_file_data::wav_file_data(const char* filename)
{
  std::ifstream fileReader(filename, std::ifstream::binary);
  if( fileReader.fail() )
  {
    char* error = strerror(errno);
    throw L"error";
  }

  header = std::make_unique<wav_file_header>(fileReader);
  format = std::make_unique<wav_file_chunk>(fileReader);
  data = std::make_unique<wav_file_chunk>(fileReader);
}

int GetWavFileDataSize(const wav_file_data& wavFileData)
{
  return wavFileData.data->size;
}
