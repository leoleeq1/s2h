#include "s2h/IO/binary.h"
#include "s2h/IO/reader.h"
#include "s2h/IO/writer.h"

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>

namespace s2h
{
std::size_t BinaryWriter::WriteUInt8(uint8_t value)
{
  fs_ << value;
  return sizeof(value);
}

std::size_t BinaryWriter::WriteUInt16(uint16_t value)
{
  fs_ << value;
  return sizeof(value);
}

std::size_t BinaryWriter::WriteUInt32(uint32_t value)
{
  fs_ << value;
  return sizeof(value);
}

std::size_t BinaryWriter::WriteUInt64(uint64_t value)
{
  fs_ << value;
  return sizeof(value);
}

std::size_t BinaryWriter::WriteInt8(int8_t value)
{
  fs_ << value;
  return sizeof(value);
}

std::size_t BinaryWriter::WriteInt16(int16_t value)
{
  fs_ << value;
  return sizeof(value);
}

std::size_t BinaryWriter::WriteInt32(int32_t value)
{
  fs_ << value;
  return sizeof(value);
}

std::size_t BinaryWriter::WriteInt64(int64_t value)
{
  fs_ << value;
  return sizeof(value);
}

std::size_t BinaryWriter::WriteFloat(float value)
{
  fs_ << value;
  return sizeof(value);
}

std::size_t BinaryWriter::WriteDouble(double value)
{
  fs_ << value;
  return sizeof(value);
}

std::size_t BinaryWriter::WriteLongDouble(long double value)
{
  fs_ << value;
  return sizeof(value);
}

std::size_t BinaryWriter::WriteString(std::string_view str)
{
  fs_ << str;
  return str.size() + sizeof(decltype(str.size()));
}

std::size_t BinaryWriter::WriteBytes(std::span<const uint8_t> bytes)
{
  fs_.write(reinterpret_cast<char *>(&bytes),
    static_cast<std::streamsize>(bytes.size_bytes()));
  return bytes.size_bytes();
}

std::size_t BinaryReader::ReadUInt8(uint8_t *value)
{
  fs_.read(reinterpret_cast<char *>(value), sizeof(decltype(*value)));
  return sizeof(decltype(*value));
}

std::size_t BinaryReader::ReadUInt16(uint16_t *value)
{
  fs_.read(reinterpret_cast<char *>(value), sizeof(decltype(*value)));
  return sizeof(decltype(*value));
}

std::size_t BinaryReader::ReadUInt32(uint32_t *value)
{
  fs_.read(reinterpret_cast<char *>(value), sizeof(decltype(*value)));
  return sizeof(decltype(*value));
}

std::size_t BinaryReader::ReadUInt64(uint64_t *value)
{
  fs_.read(reinterpret_cast<char *>(value), sizeof(decltype(*value)));
  return sizeof(decltype(*value));
}

std::size_t BinaryReader::ReadInt8(int8_t *value)
{
  fs_.read(reinterpret_cast<char *>(value), sizeof(decltype(*value)));
  return sizeof(decltype(*value));
}

std::size_t BinaryReader::ReadInt16(int16_t *value)
{
  fs_.read(reinterpret_cast<char *>(value), sizeof(decltype(*value)));
  return sizeof(decltype(*value));
}

std::size_t BinaryReader::ReadInt32(int32_t *value)
{
  fs_.read(reinterpret_cast<char *>(value), sizeof(decltype(*value)));
  return sizeof(decltype(*value));
}

std::size_t BinaryReader::ReadInt64(int64_t *value)
{
  fs_.read(reinterpret_cast<char *>(value), sizeof(decltype(*value)));
  return sizeof(decltype(*value));
}

std::size_t BinaryReader::ReadFloat(float *value)
{
  fs_.read(reinterpret_cast<char *>(value), sizeof(decltype(*value)));
  return sizeof(decltype(*value));
}

std::size_t BinaryReader::ReadDouble(double *value)
{
  fs_.read(reinterpret_cast<char *>(value), sizeof(decltype(*value)));
  return sizeof(decltype(*value));
}

std::size_t BinaryReader::ReadLongDouble(long double *value)
{
  fs_.read(reinterpret_cast<char *>(value), sizeof(decltype(*value)));
  return sizeof(decltype(*value));
}

std::size_t BinaryReader::ReadString(std::string& str)
{
  fs_ >> str;
  return str.size() + sizeof(decltype(str.size()));
}

std::size_t BinaryReader::ReadBytes(std::span<uint8_t> bytes)
{
  fs_.read(reinterpret_cast<char *>(&bytes),
    static_cast<std::streamsize>(bytes.size_bytes()));
  return bytes.size_bytes();
}
} // namespace s2h
