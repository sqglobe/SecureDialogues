#include "skipcertverifier.h"

void SkipCertVerifier::verify(
    const vmime::shared_ptr<vmime::security::cert::certificateChain>&,
    const vmime::string&) {}
