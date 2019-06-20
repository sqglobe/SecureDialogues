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
};

#endif  // ASYMETRICALKEYGENERATOR_H
