#ifndef SKIPCERTVERIFIER_H
#define SKIPCERTVERIFIER_H

#include <vmime/vmime.hpp>

class SkipCertVerifier : public vmime::security::cert::certificateVerifier {
 public:
  void verify(const vmime::shared_ptr<vmime::security::cert::certificateChain>&,
              const vmime::string&) override;
};

#endif  // SKIPCERTVERIFIER_H
