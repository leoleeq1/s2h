#ifndef S2H_IO_SERIALIZE_H_
#define S2H_IO_SERIALIZE_H_

namespace s2h
{
class IWriter;
class IReader;

template<typename T> class ISerializable
{
 public:
  virtual ~ISerializable() = default;
  virtual void Serialize(IWriter& writer) const = 0;
  virtual void Deserialize(IReader& reader) = 0;
};
} // namespace s2h

#endif // S2H_IO_SERIALIZE_H_