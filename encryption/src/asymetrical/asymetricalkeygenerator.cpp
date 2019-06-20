#include "asymetricalkeygenerator.h"
#include "asymetricalkeystore.h"

#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

AsymetricalKeyGenerator::AsymetricalKeyGenerator(
    std::shared_ptr<AsymetricalKeyStore> key) :
    mKeyStore(std::move(key)) {}

bool AsymetricalKeyGenerator::generate() {
  try {
    CryptoPP::AutoSeededRandomPool rng;

    CryptoPP::InvertibleRSAFunction parameters;
    parameters.GenerateRandomWithKeySize(rng, 3072);

    CryptoPP::RSA::PrivateKey privateKey(parameters);
    CryptoPP::RSA::PublicKey publicKey(parameters);

    mKeyStore->setKeys(publicKey, privateKey);
    return true;
  } catch (std::exception& ex) {
    spdlog::get("root_logger")
        ->error("Failed generate keys with exception {0}", ex.what());
    return false;
  }
}
