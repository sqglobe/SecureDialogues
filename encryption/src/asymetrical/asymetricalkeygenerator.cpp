#include "asymetricalkeygenerator.h"
#include "asymetricalkeystore.h"

#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

std::shared_ptr<spdlog::logger> AsymetricalKeyGenerator::LOGGER =
    spdlog::stdout_color_mt("asymetrical-key-generator");

AsymetricalKeyGenerator::AsymetricalKeyGenerator(
    const std::shared_ptr<AsymetricalKeyStore>& key) :
    mKeyStore(key) {}

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
    LOGGER->error("Failed generate keys with exception {0}", ex.what());
    return false;
  }
}
