#ifndef S2H_IO_READER_H_
#define S2H_IO_READER_H_

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>

namespace s2h
{
class IReader
{
 public:
  virtual std::size_t ReadUInt8(uint8_t *value) = 0;
  virtual std::size_t ReadUInt16(uint16_t *value) = 0;
  virtual std::size_t ReadUInt32(uint32_t *value) = 0;
  virtual std::size_t ReadUInt64(uint64_t *value) = 0;

  virtual std::size_t ReadInt8(int8_t *value) = 0;
  virtual std::size_t ReadInt16(int16_t *value) = 0;
  virtual std::size_t ReadInt32(int32_t *value) = 0;
  virtual std::size_t ReadInt64(int64_t *value) = 0;

  virtual std::size_t ReadFloat(float *value) = 0;
  virtual std::size_t ReadDouble(double *value) = 0;
  virtual std::size_t ReadLongDouble(long double *value) = 0;

  virtual std::size_t ReadString(std::string& str) = 0;

  virtual std::size_t ReadBytes(std::span<uint8_t> bytes) = 0;
};
} // namespace s2h

#endif // S2H_IO_READER_H_