#include "passask.h"
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

#include <fstream>
#include <sstream>

#include <QInputDialog>
#include <QMessageBox>

#include <libintl.h>

std::string makeDigest(const std::string& str) {
  using namespace CryptoPP;
  SHA256 hash;
  std::string digest;

  StringSource s(str, true,
                 new HashFilter(hash, new HexEncoder(new StringSink(digest))));

  return digest;
}

bool isPassValid(const std::string& pass, const std::string& passFile) {
  auto digest = makeDigest(pass);
  std::ifstream f(passFile.c_str());
  if (!f.good()) {
    std::ofstream out(passFile.c_str());
    out << digest << std::flush;
    return true;
  }
  std::string str((std::istreambuf_iterator<char>(f)),
                  std::istreambuf_iterator<char>());
  return str == digest;
}

std::string getPassword(const std::string& passFile) {
  while (true) {
    bool ok;
    auto text = QInputDialog::getText(
        nullptr, "SecureDialogues",
        dgettext("SecureDialogues",
                 "Please, enter your password for application access"),
        QLineEdit::Normal, QString(), &ok);

    if (!ok) {
      throw std::runtime_error(
          dgettext("SecureDialogues", "Application execution was canceled!"));
    }

    if (isPassValid(text.toStdString(), passFile)) {
      return text.toStdString();
    }
  }
}
