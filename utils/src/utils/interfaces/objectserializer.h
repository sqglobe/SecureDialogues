#ifndef OBJECTSERIALIZER_H
#define OBJECTSERIALIZER_H
template <typename T>
class ObjectSerializer {
 public:
  virtual ~ObjectSerializer() = default;
  virtual void serialize(const T& object) noexcept(false) = 0;
};

#endif  // OBJECTSERIALIZER_H
