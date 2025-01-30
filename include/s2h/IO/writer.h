#ifndef S2H_IO_WRITER_H_
#define S2H_IO_WRITER_H_

#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>

namespace s2h
{
class IWriter
{
 public:
  virtual std::size_t WriteUInt8(uint8_t value) = 0;
  virtual std::size_t WriteUInt16(uint16_t value) = 0;
  virtual std::size_t WriteUInt32(uint32_t value) = 0;
  virtual std::size_t WriteUInt64(uint64_t value) = 0;

  virtual std::size_t WriteInt8(int8_t value) = 0;
  virtual std::size_t WriteInt16(int16_t value) = 0;
  virtual std::size_t WriteInt32(int32_t value) = 0;
  virtual std::size_t WriteInt64(int64_t value) = 0;

  virtual std::size_t WriteFloat(float value) = 0;
  virtual std::size_t WriteDouble(double value) = 0;
  virtual std::size_t WriteLongDouble(long double value) = 0;

  virtual std::size_t WriteString(std::string_view str) = 0;

  virtual std::size_t WriteBytes(std::span<const uint8_t> bytes) = 0;
};
} // namespace s2h

#endif // S2H_IO_WRITER_H_