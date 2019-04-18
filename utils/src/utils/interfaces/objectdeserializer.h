#ifndef OBJECTDESERIALIZER_H
#define OBJECTDESERIALIZER_H
template <typename T>
class ObjectDeserializer {
 public:
  virtual ~ObjectDeserializer() = default;
  virtual void deserialize(T& object) noexcept(false) = 0;
};
#endif  // OBJECTDESERIALIZER_H
