#ifndef S2H_IO_BINARY_H_
#define S2H_IO_BINARY_H_

#include "s2h/IO/reader.h"
#include "s2h/IO/writer.h"

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

namespace s2h
{
class BinaryWriter : public IWriter
{
 public:
  BinaryWriter(const std::filesystem::path& path)
    : fs_(path, std::ios::out | std::ios::binary | std::ios::trunc
                  | std::ios::noreplace)
  {
  }
  virtual ~BinaryWriter() = default;

  std::size_t WriteUInt8(uint8_t value) override;
  std::size_t WriteUInt16(uint16_t value) override;
  std::size_t WriteUInt32(uint32_t value) override;
  std::size_t WriteUInt64(uint64_t value) override;

  std::size_t WriteInt8(int8_t value) override;
  std::size_t WriteInt16(int16_t value) override;
  std::size_t WriteInt32(int32_t value) override;
  std::size_t WriteInt64(int64_t value) override;

  std::size_t WriteFloat(float value) override;
  std::size_t WriteDouble(double value) override;
  std::size_t WriteLongDouble(long double value) override;

  std::size_t WriteString(std::string_view str) override;

  std::size_t WriteBytes(std::span<const uint8_t> bytes) override;

 private:
  std::ofstream fs_;
};

class BinaryReader : public IReader
{
 public:
  BinaryReader(const std::filesystem::path& path)
    : fs_(path, std::ios::in | std::ios::binary)
  {
  }

  virtual ~BinaryReader() = default;

  std::size_t ReadUInt8(uint8_t *value) override;
  std::size_t ReadUInt16(uint16_t *value) override;
  std::size_t ReadUInt32(uint32_t *value) override;
  std::size_t ReadUInt64(uint64_t *value) override;

  std::size_t ReadInt8(int8_t *value) override;
  std::size_t ReadInt16(int16_t *value) override;
  std::size_t ReadInt32(int32_t *value) override;
  std::size_t ReadInt64(int64_t *value) override;

  std::size_t ReadFloat(float *value) override;
  std::size_t ReadDouble(double *value) override;
  std::size_t ReadLongDouble(long double *value) override;

  std::size_t ReadString(std::string& str) override;

  std::size_t ReadBytes(std::span<uint8_t> bytes) override;

 private:
  std::ifstream fs_;
};
} // namespace s2h

#endif // S2H_IO_BINARY_H_