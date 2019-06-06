#ifndef ASYMETRICALKEYGENERATOR_H
#define ASYMETRICALKEYGENERATOR_H

#include <memory>

class AsymetricalKeyStore;
namespace spdlog {
class logger;
}
class AsymetricalKeyGenerator {
 public:
  explicit AsymetricalKeyGenerator(std::shared_ptr<AsymetricalKeyStore> key);

 public:
  bool generate();

 private:
  std::shared_ptr<AsymetricalKeyStore> mKeyStore;
  static std::shared_ptr<spdlog::logger> LOGGER;
};

#endif  // ASYMETRICALKEYGENERATOR_H
